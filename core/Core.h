#ifndef XG_CORE_H
#define XG_CORE_H

//c++ headers
#include "graphics/XChart.h"
#include "graphics/global.h"

#include "graphics/guide/Image.h"
#include "graphics/guide/Background.h"
#include "graphics/guide/Text.h"
#include "graphics/guide/Flag.h"
#include "graphics/guide/GuideController.h"
#include "graphics/guide/Line.h"
#include "graphics/guide/Point.h"
#include "graphics/guide/Tag.h"
#include "graphics/guide/GuideBase.h"

#include "graphics/legend/LegendController.h"

#include "graphics/animate/GeomAnimate.h"
#include "graphics/animate/Animator.h"
#include "graphics/animate/TimeLine.h"

#include "graphics/tooltip/TooltipController.h"
#include "graphics/tooltip/ToolTip.h"

#include "graphics/adjust/Stack.h"
#include "graphics/adjust/Dodge.h"

#include "graphics/shape/Image.h"
#include "graphics/shape/Text.h"
#include "graphics/shape/Circle.h"
#include "graphics/shape/Marker.h"
#include "graphics/shape/Shape.h"
#include "graphics/shape/Area.h"
#include "graphics/shape/Polyline.h"
#include "graphics/shape/Element.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Polygon.h"

#include "graphics/scale/Scale.h"
#include "graphics/scale/TimeCategory.h"
#include "graphics/scale/ScaleController.h"
#include "graphics/scale/Identity.h"
#include "graphics/scale/KLineCat.h"
#include "graphics/scale/Category.h"

#include "graphics/scale/continuous/Linear.h"
#include "graphics/scale/continuous/TimeSharingLinear.h"
#include "graphics/scale/continuous/FiveDaysLinear.h"

#include "graphics/util/BBox.h"
#include "graphics/util/Point.h"
#include "graphics/util/Path.h"
#include "graphics/util/Matrix.h"
#include "graphics/util/Color.h"
#include "graphics/util/json_util.h"
#include "graphics/util/json.h"
#include "graphics/util/Vector2d.h"

#include "graphics/canvas/CanvasImage.h"
#include "graphics/canvas/CanvasColorParser.h"
#include "graphics/canvas/CanvasFontParser.h"
#include "graphics/canvas/Cartesian.h"
#include "graphics/canvas/Container.h"
#include "graphics/canvas/Coord.h"
#include "graphics/canvas/Canvas.h"
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Polar.h"
#include "graphics/canvas/CanvasFillStrokeStyle.h"

#include "graphics/geom/Point.h"
#include "graphics/geom/Path.h"
#include "graphics/geom/Candle.h"
#include "graphics/geom/Area.h"
#include "graphics/geom/Line.h"
#include "graphics/geom/Geom.h"
#include "graphics/geom/Interval.h"

#include "graphics/geom/shape/Point.h"
#include "graphics/geom/shape/GeomShapeFactory.h"
#include "graphics/geom/shape/Candle.h"
#include "graphics/geom/shape/Area.h"
#include "graphics/geom/shape/Line.h"
#include "graphics/geom/shape/GeomShapeBase.h"
#include "graphics/geom/shape/Interval.h"
#include "graphics/geom/attr/AttrBase.h"

#include "graphics/axis/AxisController.h"

#include "graphics/event/EventController.h"

#include "graphics/interaction/InteractionBase.h"
#include "graphics/interaction/InteractionContext.h"
#include "graphics/interaction/Tap.h"
#include "graphics/interaction/Pan.h"
#include "graphics/interaction/Pinch.h"

#include "graphics/func/Command.h"
#include "graphics/func/Func.h"

#include "utils/StringUtil.h"
#include "utils/common.h"
#include "utils/Tracer.h"
#include "utils/xtime.h"

#include "nlohmann/json.hpp"
#include "bridge/AbstractBridgeRailing.h"
#include "bridge/ChartBridge.h"
#include "token/DarkModeManager.h"


#if defined(__APPLE__)
#include "ios/CoreGraphicsContext.h"
#include "ios/F2Logger.h"
#include "ios/BridgeRailingIOS.h"
#endif

#if defined(__ANDROID__)
#include "android/F2NativeJNI.h"
#include "android/JavaRef.h"
#include "android/JNIUtil.h"
#include "android/BridgeRailingAndroid.h"
#include "android/JavaF2Function.h"
#endif

#endif /* XG_CORE_H */
