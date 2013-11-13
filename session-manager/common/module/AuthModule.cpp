/**
 * AuthModule
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
#include <winpr/library.h>

#include <freerds/auth.h>

#include "appcontext/ApplicationContext.h"

#include "AuthModule.h"

namespace freerds
{
	namespace sessionmanager
	{
		namespace module
		{
			static wLog* logger_Module = WLog_Get("freerds.sessionmanager.module.authmodule");

			AuthModule::AuthModule()
				: mAuth(NULL),
				  mModuleEntry(NULL)
			{
				ZeroMemory(&mEntryPoints, sizeof(RDS_AUTH_MODULE_ENTRY_POINTS));
			}

			int AuthModule::initModule(pRdsAuthModuleEntry moduleEntry)
			{
				int status = 0;
				mModuleEntry = moduleEntry;

				ZeroMemory(&mEntryPoints, sizeof(RDS_AUTH_MODULE_ENTRY_POINTS));
				mEntryPoints.Version = RDS_AUTH_MODULE_INTERFACE_VERSION;

				status = mModuleEntry(&mEntryPoints);

				if (status < 0)
					return -1;

				mAuth = mEntryPoints.New();

				if (!mAuth)
					return -1;

				return 0;
			}

			AuthModule::~AuthModule()
			{
				if (mEntryPoints.Free)
				{
					mEntryPoints.Free(mAuth);
					mAuth = NULL;
				}
			}

			int AuthModule::logonUser(char* username, char* domain, char* password)
			{
				int status;

				if (!mEntryPoints.LogonUser)
					return -1;

				status = mEntryPoints.LogonUser(mAuth, username, domain, password);

				return status;
			}

			pRdsAuthModuleEntry AuthModule::loadModuleEntry(const char* filename)
			{
				HINSTANCE library;
				pRdsAuthModuleEntry moduleEntry;

				library = LoadLibraryA(filename);

				if (!library)
					return (pRdsAuthModuleEntry) NULL;

				moduleEntry = (pRdsAuthModuleEntry) GetProcAddress(library, RDS_AUTH_MODULE_ENTRY_POINT_NAME);

				if (moduleEntry)
					return moduleEntry;

				FreeLibrary(library);

				return (pRdsAuthModuleEntry) NULL;
			}

			AuthModule* AuthModule::loadFromFileName(const char* filename)
			{
				AuthModule* module;
				pRdsAuthModuleEntry moduleEntry;

				moduleEntry = AuthModule::loadModuleEntry(filename);

				if (!moduleEntry)
					return (AuthModule*) NULL;

				module = new AuthModule();
				module->initModule(moduleEntry);

				return module;
			}

			AuthModule* AuthModule::loadFromName(const char* name)
			{
				int length;
				char* filename;
				char* lowerName;
				char* libraryPath;
				AuthModule* module;
				pRdsAuthModuleEntry moduleEntry;

				libraryPath = APP_CONTEXT.getLibraryPath();

				lowerName = _strdup(name);
				CharLowerA(lowerName);

				length = strlen(lowerName) + strlen(libraryPath) + 64;
				filename = (char*) malloc(length + 1);

				sprintf_s(filename, length, "%s/libfreerds-auth-%s.so", libraryPath, lowerName);
				free(lowerName);

				module = AuthModule::loadFromFileName(filename);

				free(filename);

				return module;
			}
		}
	}
}
