#include <gtest/gtest.h>
#include <qingstor/QingStor.h>
#include <cucumber-cpp/autodetect.hpp>
#include <fstream>

#include <memory>
#include <string>

using cucumber::ScenarioScope;

using namespace QingStor;

struct TestObjectCtx
{
    QingStorService *pQsService;
    Bucket *pQsBucket;
    std::string bucketName;
};

struct TestMultipartUploadCtx
{
    QingStorService *pQsService;
    Bucket *pQsBucket;
    std::string bucketName;
    std::string objectKey;
    std::string uploadID;
    std::vector<ObjectPartType> objectParts;
};

struct TestBucketCtx
{
    QingStorService *pQsService;
    Bucket *pQsBucket;
};

struct TestListMultipartUploadsCtx
{
    QingStorService *pQsService;
    Bucket *pQsBucket;
    std::string bucketName;
    std::string objectKey;
    std::string uploadID;
};


static std::string strConfigPath = "";

static std::string strBucketName = "";

static std::string strZone = "";

// read necessary information from environment variables.
void init_test_config(void)
{
    // get QingStor config file path.
    strConfigPath = getenv("QINGSTOR_CONFIG_PATH");
    // get QingStor bucket name.
    strBucketName = getenv("QINGSTOR_BUCKET_NAME");
    // get QingStor zone name.
    strZone = getenv("QINGSTOR_ZONE_NAME");
    
    QingStor::SDKOptions sdkOptions;
    sdkOptions.logLevel = Debug;
    sdkOptions.logPath = "/tmp/";
    InitializeSDK(sdkOptions);
}


/*
Scenario: need to use QingStor service
When initialize QingStor service
Then the QingStor service is initialized
*/
WHEN("^initialize QingStor service$")
{
    init_test_config();
}

THEN("^the QingStor service is initialized$")
{
    EXPECT_EQ(NULL, NULL);
}

/*
# GET Service(List Buckets)
Scenario: list all buckets
	When list buckets
	Then list buckets status code is 200
*/
WHEN("^list buckets$")
{
    QingStor::QsConfig qsConfig;
    qsConfig.LoadConfigFile(strConfigPath.c_str());
    QingStorService qsService(qsConfig);
    ListBucketsInput input;
    ScenarioScope<ListBucketsOutput> contextOutput;
    QsError err = qsService.ListBuckets(input, *contextOutput);
    if (QS_ERR_NO_ERROR != err)
    {
    }
}

THEN("^list buckets status code is (\\d+)$")
{
    REGEX_PARAM(double, expected);
    ScenarioScope<ListBucketsOutput> contextOutput;
    EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}