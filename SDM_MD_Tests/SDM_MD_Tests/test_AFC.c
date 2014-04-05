//
//  test_AFC.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AFC_c
#define SDM_MD_Tests_test_AFC_c

#include "test_AFC.h"

#include "test_apple_AFC.h"
#include "test_sdm_AFC.h"

void Test_Compatibility_AFC(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	SDM_MD_TestResponse afc_conn = SDM_MD_Test_AFCConnectionCreate(apple, sdm, kResponse);
	TestCount(afc_conn)
	
	SDM_MD_TestResponse afc_device_info = SDM_MD_Test_AFCOperationCreateGetDeviceInfo(apple, sdm, kResponse);
	TestCount(afc_device_info)

	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

void Test_Functionality_AFC(SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
		
	// AMDeviceStartService Tests
	FunctionalityTestMacro(afc_conn,test_sdm_AFCConnectionCreate,sdm)
	TestCount(afc_conn)
	
	printf("\n");
	// AMDeviceSecureStartSessionedService Tests
	FunctionalityTestMacro(afc_device_info,test_sdm_AFCOperationCreateGetDeviceInfo,sdm)
	TestCount(afc_device_info)

	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

SDM_MD_TestResponse SDM_MD_Test_AFCConnectionCreate(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = kAMDSuccess; //test_apple_AMDeviceStartService(apple);
	
	kern_return_t sdm_return = test_sdm_AFCConnectionCreate(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateGetDeviceInfo(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = kAMDSuccess; //test_apple_AMDeviceStartService(apple);
	
	kern_return_t sdm_return = test_sdm_AFCOperationCreateGetDeviceInfo(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

#endif