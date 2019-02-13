#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

NS_ASSUME_NONNULL_BEGIN

@class MemfaultBluetoothDevice;
@class MemfaultDeviceInfo;
@class MemfaultOtaPackage;
@protocol MemfaultOtaDelegate;

//! Configuration dictionary key to specify your Memfault API key.
extern NSString* const kMFLTApiKey;


//! The Memfault SDK "entrypoint"
@interface Memfault : NSObject
//! Initializes the Memfault SDK. You should create one and use it across your app.
//! @param configuration A dictionary containing the configuration info.
//! Mandatory keys: kMFLTApiKey, ...
//! @param queue Delegate methods will be dispatched using this queue.
- (instancetype)init:(NSDictionary *)configuration queue:(nullable dispatch_queue_t)queue;

//! Gets a MemfaultBluetoothDevice given a CBPeripheral's identifier.
//! @param peripheralIdentifier A CBPeripheral's identifier for which to get a
- (MemfaultBluetoothDevice *)deviceFromPeripheralIdentifier:(NSUUID *)peripheralIdentifier;
@end


//! Represents a specific Bluetooth device.
@interface MemfaultBluetoothDevice : NSObject

//! The identifier that was used to create this device object.
//! @see Memfault.deviceFromPeripheralIdentifier
@property (readonly, nonnull) NSUUID *peripheralIdentifier;

//! Delegate object to be called when various OTA events occur.
@property (weak, nullable) id<MemfaultOtaDelegate> otaDelegate;

//! Information describing the device, or nil if the info had not been read yet.
//! @discussion Not yet implemented
@property (readonly, nullable) MemfaultDeviceInfo* info;

//! Reads the device information for the device.
//! @param completion Block to call when the device information request has completed or failed.
//! In case of success, the info property of the device will have been updated.
//! @discussion Not yet implemented
- (void)readInfo:(void(^)(BOOL success))completion;

//! Queries Memfault's services to find the latest OTA package for the device.
//! @discussion Not yet implemented
- (void)findLatestRelease:(void(^)(MemfaultOtaPackage* _Nullable latest,
                                   BOOL isDeviceUpToDate,
                                   NSError *_Nullable error))completion;

//! Performs an OTA operation given an update package.
//! @param package The OTA software package to update to.
//! @discussion If an OTA operation is already on-going to the device, it will be aborted first
//! before starting the new one.
- (void)performOta:(MemfaultOtaPackage *_Nonnull)package;

//! Aborts an ongoing OTA operation.
//! @return YES in case it was successfully aborted.
- (BOOL)abortOta;
@end


//! Memfault OTA State
typedef NS_ENUM(NSUInteger, MemfaultOtaState) {
    //! OTA has not yet begun.
    MemfaultOtaState_Idle = 0,

    //! OTA is progressing.
    MemfaultOtaState_Updating = 1,

    //! OTA is completed successfully.
    MemfaultOtaState_Success = 2,

    //! OTA failed due to an error.
    MemfaultOtaState_Failed = 3,

    //! OTA has been aborted by the user.
    MemfaultOtaState_Aborted = 4,
};


//! Protocol to receive state updates, progress and error callbacks of the OTA process.
@protocol MemfaultOtaDelegate
@optional
//! Called when the OTA progress changed.
//! @param device The device for which the OTA progress changed.
//! @param progress The total % progress (0-100) of the OTA progress.
- (void)otaProgressDidChange:(MemfaultBluetoothDevice *_Nonnull)device progress:(double)progress;

//! Called when the state of the OTA operation changes.
//! @param device The device for which the OTA state changed.
//! @param state The new state of the OTA operation.
- (void)otaStateDidChange:(MemfaultBluetoothDevice *_Nonnull)device state:(MemfaultOtaState)state;

//! Called when the OTA operation failed.
//! @param device The device for which the OTA state changed.
//! @param error An error object containing details about the failure.
//! @discussion *After* a call to this delegate method, a call to -[otaStateDidChange:state:] will
//! always follow with the state argument set to MemfaultOtaState_Failed.
- (void)otaFailed:(MemfaultBluetoothDevice *_Nonnull)device error:(NSError *)error;
@end


//! Information describing a device
//! @see MemfaultBluetoothDevice.readInfo
@interface MemfaultDeviceInfo : NSObject
// TODO: add more metadata
@property (readonly) NSString *appVersion;
@property (readonly) NSString *deviceId;
@end


//! An OTA package.
//! @see MemfaultBluetoothDevice.findLatestRelease
@interface MemfaultOtaPackage : NSObject
@property NSURL *location;
// TODO: add more metadata
@property NSString *releaseNotes;
@property NSString *appVersion;
@end

NS_ASSUME_NONNULL_END
