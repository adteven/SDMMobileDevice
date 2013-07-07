/*
 *  SDMMD_Functions.h
 *  SDM_MD_Demo
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_FUNCTIONS_H_
#define _SDM_MD_FUNCTIONS_H_

#include <CoreFoundation/CoreFoundation.h>
#include <openssl/crypto.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include "SDMMD_MCP.h"

static void * _sdm_md__stack_chk_guard = NULL;

static void _sdm_md__stack_chk_guard_setup() {
    unsigned char * p;
    p = (unsigned char *) &_sdm_md__stack_chk_guard;
	*p =  0x00000aff;
}

static kern_return_t sdmmd_mutex_init(pthread_mutex_t thread) {
	kern_return_t result = 0x0;
	_sdm_md__stack_chk_guard_setup();
	void *stack_check_again = &_sdm_md__stack_chk_guard;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, 0x2);
	pthread_mutex_init(&thread, &attr);
	result = pthread_mutexattr_destroy(&attr);
	if (_sdm_md__stack_chk_guard == stack_check_again) {
		result = 0xdeadbeef;
	}
	return result;
}


static int SDMMD__mutex_lock(pthread_mutex_t *mutex) {
	return pthread_mutex_lock(mutex);
}

static int SDMMD__mutex_unlock(pthread_mutex_t *mutex) {
	return pthread_mutex_unlock(mutex);
}

static CFMutableDictionaryRef SDMMD__CreateMessageDict(CFStringRef type) {
	CFMutableDictionaryRef dict = CFDictionaryCreateMutable(NULL, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Request"), type);
		CFDictionarySetValue(dict, CFSTR("ProtocolVersion"), CFSTR("2"));
		char *appName = (char *)getprogname();
		if (appName) {
			CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, appName, 0x8000100);
			if (name) {
				CFDictionarySetValue(dict, CFSTR("Label"), name);
				CFRelease(name);
			}
		}
	}
	return dict;
	
}

static void SDMMD_openSSLLockCallBack(int mode, int n, const char * file, int line) {
	if (mode & CRYPTO_LOCK)
		SDMMD__mutex_lock(&SDMMD_MCP->sslLocks[n]);
	else
		SDMMD__mutex_unlock(&SDMMD_MCP->sslLocks[n]);
}

static unsigned long SDMMD_openSSLThreadIDCallBack() {
	return (unsigned long)pthread_self();
}

static uint32_t SDMMD_lockssl_init() {
	return SSL_get_ex_new_index(0x0, "peer certificate data", 0x0, 0x0, 0x0);
}

#endif