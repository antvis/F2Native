#include "TimeLine.h"
#include "graphics/XChart.h"
#include "utils/xtime.h"

using namespace xg;

animate::TimeLine::~TimeLine() {
    this->animInfos_.clear();
    Stop();
}

void animate::TimeLine::Play() {
    this->time_ = CurrentTimestampAtMM();
    this->playing_ = true;
    this->frameLoop();
}

void animate::TimeLine::frameLoop() {
    if(this->playing_ == false)
        return;

    func::Command *cmd = func::CreateCommand([this]() -> void {
        if(playing_ == false)
            return;
        this->OnUpdate();
        this->frameLoop();
    });

    chart_->RequestAnimationFrame(cmd);
}

void animate::TimeLine::Stop() { this->playing_ = false; }

void animate::TimeLine::PushAnim(animate::AnimInfo &&animInfo) {
    if(!playing_) {
        Play();
    }
    animInfo.startTime = this->time_ + animInfo.delay;
    animInfo.endTime = animInfo.startTime + animInfo.duration;
    this->animInfos_.push_back(std::move(animInfo));
}

void animate::TimeLine::OnUpdate() {
    auto currTime = CurrentTimestampAtMM();
    if(animInfos_.empty()) {
        this->Stop();
        return;
    }

    std::for_each(animInfos_.begin(), animInfos_.end(), [&](AnimInfo &animInfo) -> void {
        if(currTime < animInfo.startTime || animInfo.hasEnded) {
            return;
        }

        if(animInfo.shape == nullptr || animInfo.shape->IsDestroyed()) {
            return;
        }

        if(currTime >= animInfo.startTime && !animInfo.hasStarted) {
            animInfo.hasStarted = true;
            // add callback
        }

        double t = (currTime - animInfo.startTime) / static_cast<double>(animInfo.duration);
        t = fmax(0, fmin(t, 1.));
        t = erasing::DoErasing(animInfo.erasing, t);

        {
            // OnFrame
            if(animInfo.shape != nullptr) {
                if(animInfo.interpolate.matrix != nullptr && animInfo.interpolate.matrix.get() != nullptr) {
                    util::Matrix value;
                    util::MatrixUtil::Reset(&value);
                    animInfo.interpolate.matrix->interval(&value, t);
                    animInfo.shape->SetMatrix(value);

                } else if(animInfo.interpolate.points != nullptr) {
                    // TODO points
                } else if(!animInfo.interpolate.attrs.empty()) {
                    // other number attribute.
                    for(auto it = animInfo.interpolate.attrs.begin(); it != animInfo.interpolate.attrs.end(); it++) {
                        std::string attrName = it->first;
                        double v;
                        it->second->interval(&v, t);
                        animInfo.shape->UpdateAttribute(attrName, v);
                    }
                }
            }
        }

        if(currTime >= animInfo.endTime && !animInfo.hasEnded) {
            std::function<void()> cb = animInfo.onEnd;
            cb();
            animInfo.hasEnded = true;
        }
    });

    chart_->Redraw();
    time_ = CurrentTimestampAtMM();

    // remove if anim ended.
    animInfos_.erase(std::remove_if(animInfos_.begin(), animInfos_.end(),
                                    [&](auto &animInfo) -> bool { return animInfo.hasEnded; }),
                     animInfos_.end());
}
