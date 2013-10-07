/**
 * Signaling queue template
 *
 * Copyright 2013 Thinstuff Technologies GmbH
 * Copyright 2013 DI (FH) Martin Haimberger <martin.haimberger@thinstuff.at>
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

#ifndef SIGNALINGQUEUE_H_
#define SIGNALINGQUEUE_H_

#include <list>
#include <winpr/synch.h>

template<typename QueueElement> class SignalingQueue {
public:
	SignalingQueue() {
		mSignalHandle = CreateEvent(NULL,TRUE,FALSE,NULL);
		mMutex = CreateMutex(NULL,FALSE,NULL);
	}

	~SignalingQueue() {
		CloseHandle(mSignalHandle);
		CloseHandle(mMutex);
	}

	HANDLE getSignalHandle() {
		return mSignalHandle;
	}

	void addElement(QueueElement * element) {
		WaitForSingleObject(mMutex, INFINITE);
		mlist.push_back(element);
		SetEvent(mSignalHandle);
		ReleaseMutex(mMutex);
	}

	void lockQueue() {
		WaitForSingleObject(mMutex, INFINITE);
	}

	QueueElement * getElementLockFree() {
		QueueElement * element;
		element = mlist.front();
		mlist.pop_front();
		return element;
	}

	void unlockQueue () {
		ReleaseMutex(mMutex);
	}

private:
	HANDLE mSignalHandle;
	HANDLE mMutex;
	std::list<QueueElement *> mlist;
};




#endif /* SIGNALINGQUEUE_H_ */
