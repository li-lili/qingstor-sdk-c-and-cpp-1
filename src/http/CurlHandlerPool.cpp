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

#include "plog/Log.h"
#include "http/CurlHandlerPool.h"

bool CurlHandlerPool::Init()
{
  if (0 != pthread_mutex_init(&mLock, NULL)) {
    LOGE << "Init curl handlers lock failed";
    return false;
  }

  mHandlers = new CURL*[mMaxHandlers]();
  for (int i = 0; i < mMaxHandlers; ++i, ++mIndex) {
    mHandlers[i] = curl_easy_init();
    if (!mHandlers[i]) {
      LOGE << "Init curl handlers pool failed";
      Destroy();
      return false;
    }
  }

  return true;
}

bool CurlHandlerPool::Destroy()
{
  for (int i = 0; i <= mIndex; ++i) {
    curl_easy_cleanup(mHandlers[i]);
  }
  delete[] mHandlers;

  if (0 != pthread_mutex_destroy(&mLock)) {
    LOGE << "Destroy curl handlers lock failed";
    return false;
  }

  return true;
}

CURL* CurlHandlerPool::GetHandler()
{
  CURL* h = NULL;

  pthread_mutex_lock(&mLock);
  if (mIndex >= 0) {
    LOGD << "Get handler from pool: " << mIndex;
    h = mHandlers[mIndex--];
  }
  pthread_mutex_unlock(&mLock);

  if (!h) {
    LOGI << "Pool empty: create new handler";
    h = curl_easy_init();
  }

  return h;
}

void CurlHandlerPool::ReturnHandler(CURL* h)
{
  bool needCleanup = true;

  pthread_mutex_lock(&mLock);
  if (mIndex < mMaxHandlers - 1) {
    mHandlers[++mIndex] = h;
    needCleanup = false;
    LOGD << "Return handler to pool: %d", mIndex;
  }
  pthread_mutex_unlock(&mLock);

  if (needCleanup) {
    LOGI << "Pool full: destroy the handler";
    curl_easy_cleanup(h);
  }
}