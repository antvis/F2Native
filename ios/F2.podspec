#
#  Be sure to run `pod spec lint F2.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see https://guides.cocoapods.org/syntax/podspec.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |spec|

  # ―――  Spec Metadata  ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  These will help people to find your library, and whilst it
  #  can feel like a chore to fill in it's definitely to your advantage. The
  #  summary should be tweet-length, and the description more in depth.
  #

  spec.name         = "F2"
  spec.version      = "1.0.0"
  spec.summary      = "An elegant, interactive and flexible native charting library for mobile."

  # This description is used to generate tags and improve search results.
  #   * Think: What does it do? Why did you write it? What is the focus?
  #   * Try to keep it short, snappy and to the point.
  #   * Write the description between the DESC delimiters below.
  #   * Finally, don't worry about the indent, CocoaPods strips it!
  spec.description  = "F2Native is born for mobile visual development. It is out-of-the-box,cross-platform, high-performance visualization solution . Support Android and iOS perfect,moreover have high-performance experience in low-end mobile. Based on the grammar of graphics, F2Native provides all the chart types you'll need. Our mobile design guidelines enable better user experience in mobile visualzation projects."

  spec.homepage     = "https://f2native.antv.vision"
  # spec.screenshots  = "https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*97sBRoNWXoIAAAAAAAAAAAAAARQnAQ", "https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*kWF0TYboysoAAAAAAAAAAAAAARQnAQ"


  # ―――  Spec License  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Licensing your code is important. See https://choosealicense.com for more info.
  #  CocoaPods will detect a license file if there is a named LICENSE*
  #  Popular ones are 'MIT', 'BSD' and 'Apache License, Version 2.0'.
  #

  #spec.license      = "MIT License"
  spec.license      = { :type => "MIT", :file => "../LICENSE" }


  # ――― Author Metadata  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Specify the authors of the library, with email addresses. Email addresses
  #  of the authors are extracted from the SCM log. E.g. $ git log. CocoaPods also
  #  accepts just a name if you'd rather not provide an email address.
  #
  #  Specify a social_media_url where others can refer to, for example a twitter
  #  profile URL.
  #

  spec.author             = { "weiqingtangx" => "razer751@163.com" }
  # Or just: spec.author    = "weiqingtangx"
  # spec.authors            = { "weiqingtangx" => "razer751@163.com" }
  # spec.social_media_url   = "https://twitter.com/weiqingtangx"

  # ――― Platform Specifics ――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  If this Pod runs only on iOS or OS X, then specify the platform and
  #  the deployment target. You can optionally include the target after the platform.
  #

   spec.platform     = :ios
   spec.platform     = :ios, "9.0"

  #  When using multiple platforms
  # spec.ios.deployment_target = "5.0"
  # spec.osx.deployment_target = "10.7"
  # spec.watchos.deployment_target = "2.0"
  # spec.tvos.deployment_target = "9.0"


  # ――― Source Location ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Specify the location from where the source should be retrieved.
  #  Supports git, hg, bzr, svn and HTTP.
  #

  spec.source       = { :git => "https://github.com/antvis/F2Native.git", :tag => "1.0.0",:submodules => true }


  # ――― Source Code ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  CocoaPods is smart about how it includes source code. For source files
  #  giving a folder will include any swift, h, m, mm, c & cpp files.
  #  For header files it will include any header in the folder.
  #  Not including the public_header_files will make all headers public.
  #

  spec.source_files  = "Wrapper/*.{h,m,mm}", "../core/**/*.{h,m,cpp,mm}"
  spec.exclude_files = "Classes/Exclude"

  spec.public_header_files = "Wrapper/*.h"


  # ――― Resources ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  A list of resources included with the Pod. These are copied into the
  #  target bundle with a build phase script. Anything else will be cleaned.
  #  You can preserve files from being cleaned, please don't preserve
  #  non-essential files like tests, examples and documentation.
  #

  # spec.resource  = "icon.png"
  # spec.resources = "Resources/*.png"

  # spec.preserve_paths = "FilesToSave", "MoreFilesToSave"


  # ――― Project Linking ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Link your library with frameworks, or libraries. Libraries do not include
  #  the lib prefix of their name.
  #

  # spec.vendored_frameworks  = "GCanvas"
  spec.frameworks = "GCanvas"

  # spec.library   = "iconv"
  # spec.libraries = "iconv", "xml2"


  # ――― Project Settings ――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  If your library depends on compiler flags you can set them in the xcconfig hash
  #  where they will only apply to your library. If you depend on other Podspecs
  #  you can include multiple dependencies to ensure it works.

  spec.requires_arc = true

  spec.xcconfig = { "HEADER_SEARCH_PATHS" => "$(SRCROOT)/../core" "$(SRCROOT)/../third_party" "$(SRCROOT)/../third_party/json/single_include" "$(SRCROOT)/../third_party/GCanvas/core" "$(SRCROOT)/../third_party/GCanvas/support" "$(SRCROOT)/../third_party/GCanvas/core/src" "$(SRCROOT)/../third_party/GCanvas/core/src/gcanvas" "$(SRCROOT)/../third_party/GCanvas/core/src/support" "$(SRCROOT)/../third_party/GCanvas/core/src/gcanvas/GL" "$(SRCROOT)/../third_party/GCanvas/core/src/platform"}
  # spec.dependency "JSONKit", "~> 1.4"

  spec.prefix_header_file = 'Wrapper/F2-prefix.pch'
  # spec.public_header_files = {"$(SRCROOT)/../core" "$(SRCROOT)/../third_party" "$(SRCROOT)/../third_party/json/single_include" "$(SRCROOT)/../third_party/GCanvas/core" "$(SRCROOT)/../third_party/GCanvas/support" "$(SRCROOT)/../third_party/GCanvas/core/src" "$(SRCROOT)/../third_party/GCanvas/core/src/gcanvas" "$(SRCROOT)/../third_party/GCanvas/core/src/support" "$(SRCROOT)/../third_party/GCanvas/core/src/gcanvas/GL" "$(SRCROOT)/../third_party/GCanvas/core/src/platform"}
end
