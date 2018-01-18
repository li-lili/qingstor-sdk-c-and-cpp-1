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

#include <curl/curl.h>
#include <pthread.h>

class CurlHandlerPool
{
public:
  explicit CurlHandlerPool(int maxHandlers)
    : mMaxHandlers(maxHandlers)
    , mHandlers(NULL)
    , mIndex(-1)
  {
  }

  bool Init();
  bool Destroy();

  CURL* GetHandler();
  void ReturnHandler(CURL* h);

private:
  int mMaxHandlers;

  pthread_mutex_t mLock;
  CURL** mHandlers;
  int mIndex;
};
