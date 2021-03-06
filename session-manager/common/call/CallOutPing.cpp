/**
 * Class for ping rpc call (session manager to freerds)
 *
 * Copyright 2013 Thincast Technologies GmbH
 * Copyright 2013 DI (FH) Martin Haimberger <martin.haimberger@thincast.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CallOutPing.h"

using freerds::icp::PingRequest;
using freerds::icp::PingResponse;

namespace freerds
{
	namespace sessionmanager
	{
		namespace call
		{

		CallOutPing::CallOutPing()
		{
			mPong = false;
		};

		CallOutPing::~CallOutPing()
		{

		};

		unsigned long CallOutPing::getCallType()
		{
			return freerds::icp::Ping;
		};

		int CallOutPing::encodeRequest()
		{
			// decode protocol buffers
			PingRequest req;

			if (!req.SerializeToString(&mEncodedRequest))
			{
				// failed to serialize
				mResult = 1;
				return -1;
			}

			return 0;
		};

		int CallOutPing::decodeResponse()
		{
			// encode protocol buffers
			PingResponse resp;

			if (!resp.ParseFromString(mEncodedResponse))
			{
				// failed to serialize
				mResult = 1;
				return -1;
			}
			mPong = resp.pong();
			return 0;
		};

		bool CallOutPing::getPong() {
			return mPong;
		}

}
	}
}
