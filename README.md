# Memfault iOS SDK

## Integration Guide

### Adding Memfault.framework to your project

#### CocoaPods

In case you are using CocoaPods, you can add `Memfault` as a dependency to your `Podfile`:

```
target 'MyApp' do
  pod 'Memfault'
end
```

It's probably a good idea to specify the version to use. See the [Podfile documentation] for more
information. 

After adding the new dependency, run `pod install` inside your terminal, or from CocoaPods.app.

#### Carthage

> TODO

#### Without dependency manager

To use `Memfault.framework` without using a dependency manager such as CocoaPods, just clone this
repo and drag `Memfault.framework` into your project. Make sure to link the framework to your target:
Select the Project > Select your app target > Build Phases > Link Binary With Libraries >
Add `Memfault.framework` in case it has not been added there.

### Initialization

The Memfault class is the "entrypoint" to the SDK. It is recommended to create only one Memfault
instance and use it across your entire application.

When creating the instance, you will need to pass
in a configuration dictionary. The Project API Key is the only mandatory piece of configuration. To find
your Project API Key, log in to https://app.memfault.com/ and navigate to Settings

You also need to pass in a dispatch queue. This queue will be used to execute asynchronous calls,
such as delegate method calls.

```swift
import Memfault

let memfault: Memfault = Memfault.init([
        kMFLTApiKey: "8badf00d8badf00d8badf00d8badf00d",
        ], queue:queue)
```

### Over The Air (OTA) Updates

One of the features of the Memfault SDK is updating software over the air, using Bluetooth.
There are a couple simple steps to get Memfault OTA integrated in your app:

#### 1. Getting a device object

Most likely, your app already knows how to find (scan for) the devices that your app works with.
Therefore, you will likely have a CoreBluetooth `CBPeripheral` instance in your application that
represents the device to connect to. The Memfault object has a `.device(fromPeripheralIdentifier:)`
method to derive a device object from the identifier of the peripheral:

```swift
let peripheral: CBPeripheral = ...
let device: MemfaultBluetoothDevice =
    memfault.device(fromPeripheralIdentifier: peripheral.identifier)
```

#### 2. Checking for updates

Once you have the device object, you can check to see whether there's a new update for this
particular device:

```swift
device.check { (package, error) in
    if error != nil {
        // There was an error, handle it here.
        return
    }
    if package == nil {
        // Device is up to date, nothing to do!
        return
    }
    // There's a new software package
}

```

#### 3. Perform OTA

Triggering the actual update process itself is done using the `.performOta(package)` method:

```swift
device.check { (package, error) in
    // ... error & up-to-date checks here (see previous section) ...

    device.performOta(package)
}
```

#### 4. Monitoring progress and state changes

By implementing the `MemfaultOtaDelegate` protocol, you can monitor the progress of the OTA update.
These are the delegate methods you can implement:

```swift
// Required:
func otaStateDidChange(_ device: MemfaultBluetoothDevice, state: MemfaultOtaState)

// Optional:
func otaProgressDidChange(_ device: MemfaultBluetoothDevice, progress: Double)
func otaFailed(_ device: MemfaultBluetoothDevice, error: Error)
```

Point the `.otaDelegate` property on the device object to your delegate object to receive callbacks
to these delegate methods:

```swift
device.otaDelegate = self
```


[Podfile documentation]: https://guides.cocoapods.org/syntax/podfile.html#pod
