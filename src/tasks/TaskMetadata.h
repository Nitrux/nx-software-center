//
// Created by alexis on 22/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_TASKMETADATAFIELDS_H
#define NOMAD_SOFTWARE_CENTER_TASKMETADATAFIELDS_H

namespace TaskMetadata {
    static constexpr const char *KEY_STATUS = "status";

    static constexpr const char *VALUE_STATUS_CREATED = "created";
    static constexpr const char *VALUE_STATUS_RUNNING = "running";
    static constexpr const char *VALUE_STATUS_COMPLETED = "completed";
    static constexpr const char *VALUE_STATUS_FAILED = "failed";


    static constexpr const char *KEY_TYPE = "type";
    static constexpr const char *VALUE_TYPE_DELPOY = "deploy";
    static constexpr const char *VALUE_TYPE_REMOVE = "remove";
    static constexpr const char *VALUE_TYPE_UPDATE = "update";

    static constexpr const char *KEY_DESCRIPTION = "description";

    static constexpr const char *KEY_PROGRESS_VALUE = "progress_value";
    static constexpr const char *KEY_PROGRESS_TOTAL = "progress_total";
    static constexpr const char *KEY_PROGRESS_MESSAGE = "progress_message";

    static constexpr const char *KEY_APP_ID = "task_application_id";
    static constexpr const char *KEY_APP_CODENAME = "task_application_codename";
    static constexpr const char *KEY_APP_VERSION = "task_application_version";
    static constexpr const char *KEY_APP_NAME = "task_application_name";
    static constexpr const char *KEY_APP_AUTHOR = "task_application_author";

    static constexpr const char *KEY_INSTALLATION_PATH = "task_installed_files";
    static constexpr const char *KEY_DOWNLOAD_URL = "task_download_url";
}
#endif //NOMAD_SOFTWARE_CENTER_TASKMETADATAFIELDS_H
