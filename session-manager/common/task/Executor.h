/**
 * Executor which runs Task objects
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

#ifndef __EXECUTOR_H_
#define __EXECUTOR_H_

#include "Task.h"
#include <utils/SignalingQueue.h>
#include <list>

#define PIPE_BUFFER_SIZE	0xFFFF

namespace freerds
{
	namespace task
	{
		class Executor
		{
		public:
			Executor();
			~Executor();

			int start();
			int stop();

			void runExecutor();

			void addTask(TaskPtr task);


		private:
			static void* execThread(void* arg);

		private:
			HANDLE mhStopEvent;
			HANDLE mhServerThread;
			CRITICAL_SECTION mCSection;
			SignalingQueue<TaskPtr> mTasks;
		};
	}
}

namespace taskNS = freerds::task;

#endif /* __EXECUTOR_H_ */
