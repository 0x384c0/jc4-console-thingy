#pragma once
#include <inttypes.h>
#include <stdio.h>

namespace FileLogger{

	static FILE* getFile() {
        return fopen("CustomlogFile.txt", "a");
    }

	static void log(char const* const message){
		auto pFile = getFile();
		fprintf(pFile, "   \n");
		fprintf(pFile, message);
		fclose(pFile);
	}

	static void log(char const* const message, uintptr_t pointer){
		auto pFile = getFile();
		fprintf(pFile, "    \n");
		fprintf(pFile, message);
		fprintf(pFile, " pointer: %" PRIxPTR "", pointer);
		fclose(pFile);
	}


	template <typename... Args>
	static void log_formatted(const std::string& format, Args... args)
    {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args...);
        std::string string = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

		log(string.c_str());
    }


} // namespace FileLogger


/*
Usage

#include "FileLogger.h"
    
    call logPlayer() in CPlayerManager.instance

    static void logPlayer() {

        auto manager = **(CPlayerManager**)0x142CB2388;

        FileLogger::log("\nInfo: pointers");
        intptr_t p1 = (intptr_t)0x142A5F960; // static pointer to pointer to manager
        intptr_t p2 = *(intptr_t*)p1;        // pointer to manager
        FileLogger::log("Info: static pointer to pointer to playerManager: ", p1);
        FileLogger::log("Info: pointer to playerManager: ", p2);

        FileLogger::log("Info: CPlayerManager instance");
        FileLogger::log("Info: manager (local pointer) : ", (intptr_t)&manager);
        FileLogger::log("Info: manager.m_localPlayer: ", (intptr_t) & (manager.m_localPlayer));
        FileLogger::log("Info: manager.m_localPlayer->m_character: ",
                        (intptr_t) & (manager.m_localPlayer->m_character));
        FileLogger::log("Info: manager.m_localPlayer->m_aimControl: ",
                        (intptr_t) & (manager.m_localPlayer->m_aimControl));
    }



    call logSpanwedObject(objects) in CSpawnSystem.Spawn in hk::func_call 

    static void logSpanwedObject(const spawned_objects& objects) {
                FileLogger::log("\nInfo: spawned objects");
                FileLogger::log_formatted("objects.size %d", objects.size());
                auto pointer = objects[0].get();
                FileLogger::log("\nInfo: game object std::pointer", (intptr_t)pointer);
                }

*/