#
#  Be sure to run `pod spec lint F2.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|

  spec.name         = "F2"
  spec.version      = "2.0.13"
  spec.summary      = "An elegant, interactive and flexible native charting library for mobile."
  spec.description  = "F2Native is born for mobile visual development. It is out-of-the-box,cross-platform, high-performance visualization solution . Support Android and iOS perfect, moreover have high-performance experience in low-end mobile. Based on the grammar of graphics, F2Native provides all the chart types you'll need. Our mobile design guidelines enable better user experience in mobile visualzation projects."

  spec.homepage     = "https://f2native.antv.vision"
  spec.license      = { :type => "MIT", :file => "LICENSE" }
  spec.author             = { "weiqingtangx" => "razer751@163.com" }
  spec.source       = { :git => "https://github.com/antvis/F2Native.git", :tag => "2.0.13" }
  spec.source_files  =  "ios/F2/*", "core/graphics/**/*", "core/ios/**/*", "core/utils/**/*", "core/bridge/**/*", "core/nlohmann/**/*"
  spec.public_header_files = "ios/F2/*.{h}"
  spec.prefix_header_file = 'ios/F2/F2-prefix.pch'

  spec.ios.frameworks = 'Foundation', 'UIKit'

  spec.ios.deployment_target = '9.0'
  spec.requires_arc = true

  #保护目录结构不变，如果不设置，所有头文件都将被放到同一个目录下 当需要暴露c++头文件后在设置
  # spec.header_mappings_dir = "core/**"
end
