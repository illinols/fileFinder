#include "fileFinder.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "fileFinder_gen.cpp"
#include <filesystem>
#include <iostream> 
#include <chrono>
#include <ctime>
#include <set>
#endif


DEFINE_FIRMWARE_FB(FORTE_fileFinder, g_nStringIdfileFinder)

const CStringDictionary::TStringId FORTE_fileFinder::scm_anDataInputNames[] = { g_nStringIdDP, g_nStringIdFE };

const CStringDictionary::TStringId FORTE_fileFinder::scm_anDataInputTypeIds[] = { g_nStringIdSTRING, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_fileFinder::scm_anDataOutputNames[] = { g_nStringIdLF, g_nStringIdOut, g_nStringIdLC, g_nStringIdTR };

const CStringDictionary::TStringId FORTE_fileFinder::scm_anDataOutputTypeIds[] = { g_nStringIdSTRING, g_nStringIdINT, g_nStringIdDATE_AND_TIME, g_nStringIdBOOL };

const TDataIOID FORTE_fileFinder::scm_anEIWith[] = { 0, 255 };
const TForteInt16 FORTE_fileFinder::scm_anEIWithIndexes[] = { 0 };
const CStringDictionary::TStringId FORTE_fileFinder::scm_anEventInputNames[] = { g_nStringIdREQ };

const TDataIOID FORTE_fileFinder::scm_anEOWith[] = { 0, 255 };
const TForteInt16 FORTE_fileFinder::scm_anEOWithIndexes[] = { 0 };
const CStringDictionary::TStringId FORTE_fileFinder::scm_anEventOutputNames[] = { g_nStringIdCNF };


const SFBInterfaceSpec FORTE_fileFinder::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  2, scm_anDataInputNames, scm_anDataInputTypeIds,
  4, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

void FORTE_fileFinder::setInitialValues() {
}
void FORTE_fileFinder::alg_REQ(void) {
    static std::set<std::string> previousFileNames;

    const std::string folder = st_DP().getValue();
    const std::string filter = st_FE().getValue();

    std::string lastFileName;
    if (!filter.empty() && filter[0] != '.') {
        filter = "." + filter;
    }

    std::vector<std::filesystem::directory_entry> allFiles;
    std::vector<std::filesystem::directory_entry> filteredFiles;
    std::set<std::string> currentFileNames;

    std::error_code ec;
    try {
        if (std::filesystem::exists(folder, ec) && std::filesystem::is_directory(folder, ec)) {
            for (const auto& entry : std::filesystem::directory_iterator(folder, ec)) {
                if (!std::filesystem::is_regular_file(entry)) continue;
                allFiles.push_back(entry);
                std::string filename = entry.path().filename().string();
                currentFileNames.insert(filename);

                if (!filter.empty() && entry.path().extension() == filter) {
                    filteredFiles.push_back(entry);
                }
            }

            // Поиск новых файлов
            std::string newFileDetected;
            for (const std::string& fname : currentFileNames) {
                if (filter.empty() && previousFileNames.find(fname) == previousFileNames.end()) {
                    newFileDetected = fname;
                    break;
                }
                else if (previousFileNames.find(fname) == previousFileNames.end() && std::filesystem::path(fname).extension() == filter) {
                    newFileDetected = fname;
                    break;
                }
            }

            if (!newFileDetected.empty() && filter.empty()) {
                lastFileName = newFileDetected;
                st_LF().fromString(lastFileName.c_str());
            }
            else if (!filter.empty()) {
                lastFileName = filteredFiles.back().path().filename().string();
                st_LF().fromString(lastFileName.c_str());
            }
            else {
                lastFileName = "";
            }


            DEVLOG_INFO("Фильтр: '%s' (пустой: %s)\n", filter.c_str(), filter.empty() ? "да" : "нет");

            size_t totalFileCount = allFiles.size();
            size_t filteredFileCount = filteredFiles.size();

            st_Out() = static_cast<TForteInt32>(filter.empty() ? totalFileCount : filteredFileCount);


            if (!newFileDetected.empty() && filter.empty()) {
                st_TR() = true;
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                TForteUInt64 forteTimeMillis = static_cast<TForteUInt64>(now_c) * 1000ULL;
                CIEC_DATE_AND_TIME dt(forteTimeMillis);
                st_LC() = dt;
            }
            else if (!newFileDetected.empty() && !filter.empty()) {
                st_TR() = true;
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                TForteUInt64 forteTimeMillis = static_cast<TForteUInt64>(now_c) * 1000ULL;
                CIEC_DATE_AND_TIME dt(forteTimeMillis);
                st_LC() = dt;
            }
            else {
                st_TR() = false;
            }

            previousFileNames.clear();

            if (filter.empty()) {
                previousFileNames = std::move(currentFileNames);
            }
            else {
                for (const auto& entry : filteredFiles) {
                    previousFileNames.insert(entry.path().filename().string());
                }
            }
            
            DEVLOG_INFO("Всего файлов: %zu, Отфильтровано: %zu, Последний: %s\n",
                totalFileCount, filteredFileCount, lastFileName.c_str());
        }
        else {
            DEVLOG_WARNING("Папка не найдена: %s\n", folder.c_str());
            st_Out() = 0;
            st_TR() = false;
        }
    }
    catch (const std::exception& e) {
        DEVLOG_ERROR("Ошибка: %s\n", e.what());
        st_Out() = -1;
        st_TR() = false;
    }
    catch (...) {
        DEVLOG_ERROR("Неизвестное исключение!\n");
        st_Out() = -1;
        st_TR() = false;
    }
}