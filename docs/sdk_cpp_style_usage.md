# QingStor Service Usage Guide

Import the QingStor and initialize service with a config, and you are ready to use the initialized service. Service only contains one API, and it is "ListBuckets". To use bucket related APIs, you need to initialize a bucket from service using "Bucket" function.

Each API function take a Input struct and return an Output struct. The Input struct consists of request params, request headers, request elements and request body, and the Output holds the HTTP status code, QingStor request ID, response headers, response elements, response body and error (if error occurred).

You can use a specified version of a service by import a service package with a date suffix.

```
#include <qingstor/QingStor.h>
```

### Code Snippet

Initialize the QingStor service with a configuration

```c
static const std::string strConfigPath = "/etc/qingstor/config.yaml";

static const std::string strBucketName = "yourbucketname";

static const std::string strZone = "yourzone";

// Global initialization only needs to be initialized once
// Valid log levels are "none","debug", "info", "warn", "error", and "fatal".(default value is "None")
QingStor::SDKOptions sdkOptions;
sdkOptions.logLevel = Debug;
sdkOptions.logPath = "/tmp/";

InitializeSDK(sdkOptions);

// Create QsConfig object , and init it with config info loaded form ConfigPath.
// You can also set up the config configuration item separately.
QingStor::QsConfig qsConfig;
qsConfig.loadConfigFile(strConfigPath);
QingStorService qsService(qsConfig);

```
List buckets
``` c
ListBucketsInput input;
ListBucketsOutput output;

input.SetLimit(200);

QsError err = qsService.listBuckets(input, output);
if (QsError::QS_ERR_NO_ERROR != err)
{
    // Print the HTTP status code.
    // Example: 200
    cout<<output.GetResponseCode()<<endl;
}
```
Initialize a QingStor bucket
```c
pQsBucket = new Bucket(qsConfig, strBucketName, "zone");
```
List objects in the bucket
```c
Bucket qsBucket = qsService.GetBucket(strBucketName, "zone");

ListObjectsInput input;
ListObjectsOutput output;

input.SetLimit(200);

QsError err = qsBucket.listObjects(input, output);
if (QsError::QS_ERR_NO_ERROR != err)
{
    // Print the HTTP status code.
    // Example: 200
    cout<<output.GetResponseCode()<<endl;

    // Print the key count.
    // Example: 0
    cout<<output.GetKeys()<<endl;
}
```
Put ACL of the bucket
```c
PutBucketACLInput input;
PutBucketACLOutput output;
input.SetACL(aclList);

QsError err = qsBucket.putBucketACL(input, output);
if (QsError::QS_ERR_NO_ERROR != err)
{
    // Print the HTTP status code.
    // Example: 200
    cout<<output.GetResponseCode()<<endl;
}
```
Put object with key
```c
PutObjectInput input;
PutObjectOutput output;

// object content is come from a file stream.
std::iostream * = new objectStream(new std::fstream("/tmp/Screenshot.jpg"));
objectStream->seekg(0, objectStream->end);
auto streamSize = objectStream->tellg();
objectStream->seekg(0, objectStream->beg);
input.SetBody(objectStream);
input.SetContentLength(streamSize);

QsError err = qsBucket.putObject(objectKey, input, output);
if (QsError::QS_ERR_NO_ERROR != err)
{
    // Print the HTTP status code.
    // Example: 201
    cout<<ouput.GetResponseCode()<<endl;
}

// release the objectStream
if(objectStream)
    delete objectStream;
```
Delete object with key
```c
DeleteObjectInput input;
DeleteObjectOutput output;

QsError err = qsBucket.deleteObject(objectkey, input, output);
if (QsError::QS_ERR_NO_ERROR != err)
{
    // Print the HTTP status code.
    // Example: 204
    cout << output.GetResponseCode() << endl;
}
```
Initialize Multipart Upload
```c
initiateMultipartUpload input;
initiateMultipartUpload output;

QsError err = qsBucket.initiateMultipartUpload(objectkey, input, output);
if (QsError::QS_ERR_NO_ERROR == err)
{
    // Print the HTTP status code.
    // Example: 200
    cout<<output.GetResponseCode() << endl;
    // Print the upload ID.
    // Example: "9d37dd6ccee643075ca4e597ad65655c"
    cout<<output.GetUploadID() << endl;
}
```
Upload Multipart
```c
// Upload the first part
UploadMultipartInput inputPart1;
UploadMultipartOutput outputPart1;

std::iostream* objectStream1 = new std::fstream(filePath1));
objectStream1->seekg(0, objectStream1->end); 
size_t streamSize1 = objectStream1->tellg(); 
objectStream1->seekg(0, objectStream1->beg); 
inputPart1.SetBody(objectStream1); 
inputPart1.SetContentLength(streamSize1);
inputPart1.SetPartNumber(1);
inputPart1.SetUploadID("9d37dd6ccee643075ca4e597ad65655c");
QsError err1 = qsBucket.uploadMultipart(objectkey, inputPart1, outputPart1);
if (QsError::QS_ERR_NO_ERROR == err1)
{
    // Print the HTTP status code.
    // Example: 201
    cout << outputPart1.GetResponseCode() << endl;
}

// release the objectStream
if(objectStream1)
    delete objectStream1;

// Upload the second part
UploadMultipartInput inputPart2;
UploadMultipartOutput outputPart2;

std::iostream* objectStream2 = new std::fstream(filePath2));
objectStream2->seekg(0, objectStream2->end); 
size_t streamSize2 = objectStream2->tellg(); 
objectStream2->seekg(0, objectStream2->beg); 
inputPart2.SetBody(objectStream2); 
inputPart2.SetContentLength(streamSize2);
inputPart2.SetPartNumber(2);
inputPart2.SetUploadID("9d37dd6ccee643075ca4e597ad65655c");

QsError err2 = qsBucket.uploadMultipart(objectkey, inputPart2, outputPart2);
if (QsError::QS_ERR_NO_ERROR == err2)
{
    // Print the HTTP status code.
    // Example: 201
    cout<<outputPart1.GetResponseCode()<<endl;
}

// release the objectStream
if(objectStream2)
    delete objectStream2;
```

Complete Multipart Upload
```c
std::vector<ObjectPartType> objectParts;

ObjectPartType part1,part2;
part1.SetPartNumber(1);
part2.SetPartNumber(2);
objectParts.push_back(part1);
objectParts.push_back(part2);

CompleteMultipartUploadInput input;
CompleteMultipartUploadOutput output;
input.SetUploadID("9d37dd6ccee643075ca4e597ad65655c");
input.SetObjectParts(objectParts);

QsError err = qsBucket.completeMultipartUpload(objectkey, input, output);
if (QsError::QS_ERR_NO_ERROR == err)
{
    // Print the HTTP status code.
    // Example: 201
    cout<<output.GetResponseCode()<<endl;
}
```
Abort Multipart Upload
```c
AbortMultipartUploadInput input;
AbortMultipartUploadOutput output;
input.SetUploadID("9d37dd6ccee643075ca4e597ad65655c");

QsError err = qsBucket.abortMultipartUpload(objectkey, input, output);
if (QsError::QS_ERR_NO_ERROR == err)
{
    // Print the HTTP status code.
    // Example: 400
    cout<<output.GetResponseCode()<<endl;
}
```