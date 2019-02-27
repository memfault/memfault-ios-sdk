#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

NS_ASSUME_NONNULL_BEGIN

@class MemfaultBluetoothDevice;
@class MemfaultDeviceInfo;
@class MemfaultOtaPackage;
@protocol MemfaultOtaDelegate;

typedef NS_ENUM(NSUInteger, MemfaultLogLevel);

//! Configuration dictionary key to specify your Memfault API key.
extern NSString* const kMFLTApiKey;

//! Configuration dictionary key to specify Memfault API url to use
//! (Not needed by default)
extern NSString* const kMFLTApiBaseURL;


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

@property MemfaultLogLevel logLevel;

@end


//! Represents a specific Bluetooth device.
@interface MemfaultBluetoothDevice : NSObject

//! The identifier that was used to create this device object.
//! @see Memfault.deviceFromPeripheralIdentifier
@property (readonly, nonnull) NSUUID *peripheralIdentifier;

//! Delegate object to be called when various OTA events occur.
@property (weak, nullable) id<MemfaultOtaDelegate> otaDelegate;

//! Indicates whether an OTA process is currently running.
@property (readonly) BOOL isPerformingOta;

//! Information describing the device, or nil if the info had not been read yet.
@property (readonly, nullable) MemfaultDeviceInfo* info;

//! Reads the device information for the device.
//! @param completion Block to call when the device information request has completed or failed.
//! In case of success, error will be nil and the info property of the device will have been updated.
- (void)readInfo:(void(^)(NSError *_Nullable error))completion;

//! Queries Memfault's services to find if a new update is available
//! @param completion Block to call when a result has been determined. In
//!  the case of success, an update will be non-nil if there is a new firmware to
//!  install or nil if everything is up to date.
- (void)checkForUpdate:(void(^)(MemfaultOtaPackage* _Nullable update,
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

    //! OTA is downloading the package.
    MemfaultOtaState_Downloading = 1,

    //! OTA is progressing.
    MemfaultOtaState_Updating = 2,

    //! OTA is completed successfully.
    MemfaultOtaState_Success = 3,

    //! OTA failed due to an error.
    MemfaultOtaState_Failed = 4,

    //! OTA has been aborted by the user.
    MemfaultOtaState_Aborted = 5,
};


//! Protocol to receive state updates, progress and error callbacks of the OTA process.
@protocol MemfaultOtaDelegate <NSObject>
@required
//! Called when the state of the OTA operation changes.
//! @param device The device for which the OTA state changed.
//! @param state The new state of the OTA operation.
- (void)otaStateDidChange:(MemfaultBluetoothDevice *_Nonnull)device state:(MemfaultOtaState)state;

@optional
//! Called when the OTA progress changed.
//! @param device The device for which the OTA progress changed.
//! @param progress The total % progress (0-100) of the OTA progress.
- (void)otaProgressDidChange:(MemfaultBluetoothDevice *_Nonnull)device progress:(double)progress;

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
@property (readonly, nonnull) NSString *appVersion;
@property (readonly, nonnull) NSString *deviceId;
@property (readonly, nonnull) NSString *hwVersion;
@end


//! An OTA package.
//! @see MemfaultBluetoothDevice.checkForUpdate
@interface MemfaultOtaPackage : NSObject
@property NSURL *location;
@property NSString *releaseNotes;
@property NSString *appVersion;
@end

typedef NS_ENUM(NSUInteger, MemfaultLogLevel) {
    MemfaultLogLevel_Debug,
    MemfaultLogLevel_Info,
    MemfaultLogLevel_Warning,
    MemfaultLogLevel_Error,
};

typedef NS_ENUM(NSUInteger, MemfaultErrorCode) {
    MemfaultErrorCode_InvalidArgument = 1,
    MemfaultErrorCode_UnexpectedResponse = 11,
    MemfaultErrorCode_NotFound = 12,
    MemfaultErrorCode_NotImplemented = 13,
    MemfaultErrorCode_TransportNotAvailable = 14,
    MemfaultErrorCode_EndpointNotFound = 15,
    MemfaultErrorCode_Disconnected = 16,
    MemfaultErrorCode_Timeout = 17,
    MemfaultErrorCode_PlatformSpecificBase = 100000,
};


NS_ASSUME_NONNULL_END
