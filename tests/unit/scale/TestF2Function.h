#include "graphics/func/Func.h"

#ifndef XG_TEST_F2_FUNCTION
#define XG_TEST_F2_FUNCTION

using namespace xg;
using namespace std;

typedef std::function<const nlohmann::json &(const nlohmann::json &)> TestFunc;

struct TestF2Function : public func::F2Function {
  public:
    TestF2Function(TestFunc _call) : func::F2Function(), call_(_call) {}
    nlohmann::json Execute(nlohmann::json t = nlohmann::json()) override {
        nlohmann::json rst = call_(t);
        return rst;
    }
    ~TestF2Function() override { call_ = nullptr; }

  private:
    TestFunc call_;
};

#endif
