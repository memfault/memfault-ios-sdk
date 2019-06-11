Pod::Spec.new do |spec|
  spec.name             = 'Memfault'
  spec.version          = '0.5.0'
  spec.summary          = 'Memfault SDK for iOS'
  spec.homepage         = 'https://github.com/memfault/memfault-ios-sdk'
  spec.license          = { :type => 'Modified BSD', :file => 'LICENSE' }
  spec.author           = { 'Memfault' => 'info@memfault.io' }
  spec.source           = { :git => 'https://github.com/memfault/memfault-ios-sdk.git', :tag => spec.version.to_s }
  spec.swift_version    = '4.2'
  spec.ios.deployment_target = '10.0'
  spec.ios.vendored_frameworks = 'Memfault.framework'
  spec.frameworks = 'CoreBluetooth'
  spec.dependency 'iOSDFULibrary', '~> 4.2.2'
end
