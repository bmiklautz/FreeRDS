/**
 * Connection class
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/wlog.h>
#include <winpr/pipe.h>
#include <winpr/synch.h>
#include <winpr/thread.h>
#include <winpr/environment.h>

#include <iostream>
#include <sstream>

#include <freerds/freerds.h>

#include <appcontext/ApplicationContext.h>

#include "Connection.h"

namespace freerds
{
	namespace sessionmanager
	{
		namespace session
		{
			static DWORD gConnectionId = 1;

			static wLog* logger_Connection = WLog_Get("freerds.sessionmanager.session.connection");

			Connection::Connection(DWORD connectionId)
				: mConnectionId(connectionId),
				  mServerPipeConnected(false),
				  mClientPipeConnected(false),
				  mConnector(NULL)
			{
				std::ostringstream os;

				os << "\\\\.\\pipe\\FreeRDS_Connection_" << mConnectionId;
				mServerPipeName = os.str();
			}

			Connection::~Connection()
			{

			}

			std::string Connection::getServerPipeName()
			{
				return mServerPipeName;
			}

			std::string Connection::getClientPipeName()
			{
				return mClientPipeName;
			}

			HANDLE Connection::createServerPipe()
			{
				DWORD dwPipeMode;
				HANDLE hNamedPipe;

				freerds_named_pipe_clean(mServerPipeName.c_str());

				dwPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

				hNamedPipe = CreateNamedPipeA(mServerPipeName.c_str(), PIPE_ACCESS_DUPLEX,
						dwPipeMode, 1, PIPE_BUFFER_SIZE, PIPE_BUFFER_SIZE, 0, NULL);

				if ((!hNamedPipe) || (hNamedPipe == INVALID_HANDLE_VALUE))
				{
					WLog_Print(logger_Connection, WLOG_ERROR, "failed to create server named pipe");
					return NULL;
				}

				return hNamedPipe;
			}

			HANDLE Connection::connectClientPipe(std::string clientPipeName)
			{
				HANDLE hNamedPipe;
				mClientPipeName = clientPipeName;

				hNamedPipe = freerds_named_pipe_connect(mClientPipeName.c_str(), 20);

				if ((!hNamedPipe) || (hNamedPipe == INVALID_HANDLE_VALUE))
				{
					WLog_Print(logger_Connection, WLOG_ERROR, "failed to connect to client named pipe");
					return NULL;
				}

				return hNamedPipe;
			}

			Connection* Connection::create()
			{
				Connection* connection = new Connection(gConnectionId++);
				return connection;
			}
		}
	}
}

