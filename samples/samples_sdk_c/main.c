// +-------------------------------------------------------------------------
// | Copyright (C) 2017 Yunify, Inc.
// +-------------------------------------------------------------------------
// | Licensed under the Apache License, Version 2.0 (the "License");
// | you may not use this work except in compliance with the License.
// | You may obtain a copy of the License in the LICENSE file, or at:
// |
// | http://www.apache.org/licenses/LICENSE-2.0
// |
// | Unless required by applicable law or agreed to in writing, software
// | distributed under the License is distributed on an "AS IS" BASIS,
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// | See the License for the specific language governing permissions and
// | limitations under the License.
// +-------------------------------------------------------------------------

// +-------------------------------------------------------------------------
// | This Example shows how to use the API interface.
// | For more information, please refer to URL :
// | 
// +-------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include "qingstor/QsErrors.h"
#include "qingstor/service_with_c_style/QsList.h"
#include "qingstor/service_with_c_style/QingStorCStyle.h"
//#include <memory>

static char* strConfigPath = "";

static char* strBucketName = "";

static char* strZone = "";
/*
static std::iostream* Create5MbStream4UploadPart(const char* partTag)
{
    size_t fiveMbSize = 5 * 1024 * 1024;
    char*stream patternStream;
    patternStream << "Multi-Part upload Test Part " << partTag << ":" << std::endl;
    char* pattern = patternStream.str();
    char* scratchString;
    scratchString.reserve(fiveMbSize);
    
    // 5MB is a hard minimum for multi part uploads; make sure the final string is at least that long
    size_t patternCopyCount = static_cast< size_t >( fiveMbSize / pattern.size() + 1 );
    for(size_t i = 0; i < patternCopyCount; ++i)
    {
        scratchString.append(pattern);
    }
    std::iostream* streamPtr = new char*stream(scratchString);
    streamPtr->seekg(0);
    streamPtr->seekp(0, std::ios_base::end);
    return streamPtr;
}
*/

void ShowHow2Do_InitiateMultipartUpload(qs_context_handle context_hdl, char* objectkey, char** uploadID)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    qs_initiate_multipart_upload_input_t input;
    qs_initiate_multipart_upload_output_t output;
    init_initiate_multipart_upload_input (&input);
    *uploadID = (char*)malloc(strlen(output.upload_id)+1);
    memset((*uploadID), 0, strlen(output.upload_id));
    QsError err = qs_initiate_multipart_upload(objectkey, &input, &output, context_hdl);
    if (QS_ERR_NO_ERROR == err && output.response_code == 200)
    {
       strncpy(*uploadID,output.upload_id,strlen(output.upload_id));
       //printf("uploadID is %s\n",*uploadID);
    }
    printf("ShowHow2Do_InitiateMultipartUpload is ok\n");
    return;
}


void ShowHow2Do_UploadMultipart(qs_context_handle context_hdl, char* objectkey, char* uploadID ,int partNumber)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    qs_upload_multipart_input_t input;
    qs_upload_multipart_output_t output;
    init_upload_multipart_input (&input);

    long length = strlen (" |thi is a Part 1| ");
    
    input.bodybuf = (char *)malloc (length);
    memset(input.bodybuf, 0 , length);
    input.bufLength = &length;
    input.content_length = &length;
    input.part_number = &partNumber;
    input.upload_id = uploadID;

    QsError err = qs_upload_multipart (objectkey, &input, &output, context_hdl);
    if (QS_ERR_NO_ERROR == err)
    {
        // print sth
        printf("ShowHow2Do_UploadMultipart is OK,part %d is ok",partNumber);
    }
   
    // when api finish, you should release resource.
    if(input.bodybuf)
    {
        free(input.bodybuf);
        input.bodybuf = NULL;
    }
    
    return;
}
/*
void ShowHow2Do_CompleteMultipartUpload(qs_context_handle context_hdl, char* objectkey, char* uploadID)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    qs_complete_multipart_upload_input_t input;
    qs_complete_multipart_upload_output_t output;
    init_complete_multipart_upload_input (&input);
    input.upload_id = &uploadID;
    qs_list_t object_parts_list;
    qs_list_init (&object_parts_list);
    qs_object_part_item_t object_parts_item_1, object_parts_item_2, object_parts_item_3;
    qs_object_part_t object_parts_1, object_parts_2, object_parts_3;
    init_object_part (&object_parts_1);
    init_object_part (&object_parts_2);
    init_object_part (&object_parts_3);
    int part_number_1 = 1;
    int part_number_2 = 2;
    int part_number_3 = 3;
    object_parts_1.part_number = &part_number_1;
    object_parts_2.part_number = &part_number_2;
    object_parts_3.part_number = &part_number_3;
    object_parts_item_1.content = &object_parts_1;
    object_parts_item_2.content = &object_parts_2;
    object_parts_item_3.content = &object_parts_3;
    qs_list_append (&object_parts_item_1.node, &object_parts_list);
    qs_list_append (&object_parts_item_2.node, &object_parts_list);
    qs_list_append (&object_parts_item_3.node, &object_parts_list);

    input.object_parts = &object_parts_list;
    input.upload_id = &uploadID;

    QsError err = qs_complete_multipart_upload (objectkey, &input, &output , context_hdl);
    if (QS_ERR_NO_ERROR == err)
    {
        
    }
    
    return;
}
*/
void ShowHow2Do_ListObjects(qs_context_handle context_hdl)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    // List Objects
    qs_list_objects_input_t input;
    qs_list_objects_output_t output;
    init_list_objects_input(&input);
    // if you want limit the maximum number of object in response, you can set "Limit" paramter. 
    // the default value is 200, maximum allowable set 1000
    int limitNum = 100;
    input.limit = &limitNum;// 

    QsError err = qs_list_objects(&input, &output, context_hdl);
     if(QS_ERR_NO_REQUIRED_PARAMETER == err)
    {
        printf("%s\n" , "The input object lacks some of the parameters that must be filled in.");
    }

    if (QS_ERR_NO_ERROR != err)
    {
        return;
    }
    else{
        int responsecode = output.response_code;
        printf("the ListObject responsecode is %d\n" , responsecode);
    }
    qs_release_service(context_hdl);
 
    qs_list_t *keys = output.keys;
    int count = 0;
    qs_key_item_t *pos;
    qs_list_for_each_entry(qs_key_item_t, pos, keys)
    {
        count++;
    }
    release_list_objects_output(&output);
    printf("Got %d objects\n" ,count);

    return;
}


void ShowHow2Do_MultipartUploadObject(qs_context_handle context_hdl, char* objectkey)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    char* uploadID = "";
    printf("ok\n");
    ShowHow2Do_InitiateMultipartUpload(context_hdl , objectkey , &uploadID);

    ShowHow2Do_UploadMultipart(context_hdl, objectkey, uploadID ,1);

    ShowHow2Do_UploadMultipart(context_hdl, objectkey, uploadID ,2);

    //std::vector<ObjectPartType> objectParts;

   // ObjectPartType part1,part2;
    //part1.SetPartNumber(1);
   // part2.SetPartNumber(2);
    //objectParts.push_back(part1);
    //objectParts.push_back(part2);

    //ShowHow2Do_CompleteMultipartUpload(context_hdl, objectkey, uploadID, objectParts);
    //free(*uploadID);
    return;
}

/*
void ShowHow2Do_GetObject(qs_context_handle context_hdl, char* objectkey)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    qs_get_object_input_t input;
    qs_get_object_output_t output;
    init_get_object_input (&input);
    char * object_key = (char *)malloc(strlen(objectkey) + 1);
    memset(object_key,0,strlen(objectkey)+1);
    strncpy(object_key, objectkey, strlen(objectkey));

    QsError err = qs_get_object (object_key, &input, &output, context_hdl);
    if(QS_ERR_NO_REQUIRED_PARAMETER == err)
    {
        printf("%s\n" , "The input object lacks some of the parameters that must be filled in.");
    }
    if (QS_ERR_NO_ERROR != err)
    {
        return;
    }
    else
    {
        int responsecode = output.response_code;
        printf("the GetObject responsecode is %d\n" , responsecode);
    }

    free(object_key);

    return;
}


/*
void ShowHow2Do_DeleteObject(qs_context_handle context_hdl, char* objectkey)
{
    if(context_hdl.pQsBucket == NULL || context_hdl.pQsService == NULL){
        return;
    }

    qs_delete_object_input_t input;
    qs_delete_object_output_t output;
    init_delete_object_input (&input);
    char * object_key = (char *)malloc(strlen(objectkey) + 1);
    memset(object_key,0,strlen(objectkey) + 1);
    strncpy(object_key, objectkey , strlen(objectkey));
    QsError err = qs_delete_object (object_key, &input, &output, context_hdl);
    if(QS_ERR_NO_REQUIRED_PARAMETER == err)
    {
        printf("%s\n" , "The input object lacks some of the parameters that must be filled in.");
    }

    if (QS_ERR_NO_ERROR != err)
    {
        return;
    }
    else
    {
        int responsecode = output.response_code;
        printf("the DeleteObject responsecode is %d\n" , responsecode);
    }

    printf("object(%s) has been deleted.\n" , objectkey);

    free(object_key);

    release_delete_object_output(&output);
    
    return;

}
*/
int main()
{
    // Read necessary information from envirox  nment variables.
	
    static const char* strConfigPath = "/etc/qingstor/config.yaml";

    static const char* strBucketName = "huang-stor";

    static const char* strZone = "pek3a";

    qs_init_sdk("/tmp/", Debug, 1);

	// Global initialization only needs to be initialized once
    qs_context_handle context_hdl = qs_create_service_with_configfile(strConfigPath, strBucketName, strZone);
    // Valid log levels are "none","debug", "info", "warn", "error", and "fatal".(default value is "None")


    ShowHow2Do_ListObjects(context_hdl);  
    
    char* objectkey = "QingStor_SDK_Test_File";

    ShowHow2Do_MultipartUploadObject(context_hdl, objectkey);

    //ShowHow2Do_GetObject(context_hdl, objectkey);

    //ShowHow2Do_DeleteObject(context_hdl, objectkey);

    //ShutdownSDK(sdkOptions);
    //qs_release_service(context_hdl);

    return 0;
}






