#include "Config.hpp"

#include "detectors/DetectorList.hpp"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace {
/*
template<typename T>
void setDefaults(lint::Config* config, T) {
    config->setOptionNamed(T::kOptionName, T::kDefaultValue);
}

template<typename T, typename... Ts>
void setDefaults(lint::Config* config, T, Ts... ts) {
    config->setOptionNamed(T::kOptionName, T::kDefaultValue);
    setDefaults(config, ts...);
}
*/

template<typename T>
void setDefault(lint::Config* config) {
    config->setOptionNamed(T::kOptionName, T::kDefaultValue);
}

template<typename T, typename... Ts>
void setDefaults<lint::TypeList<T, Ts>>(lint::Config* config) {
    setDefault<T>(config);
    setDefaults<Ts...>(config);
}

template<>
void setDefaults<lint::TypeList<>>(lint::Config*) { }

} // namespace

namespace lint {

void Config::initDefaults() {
    setDefaults<DetectorList>(this);
}

std::string Config::readJSON(std::string_view jsonString) {
    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(jsonString.data());

    // Validate some basic assumptions about the input JSON.
    if (!parseResult)
        return "failed to parse input JSON.";

    if (!document.IsObject())
        return "input JSON is not a dictionary.";

    if (document.HasMember(kOneNewlineAtEndOfFileName)) {
        if (!document[kOneNewlineAtEndOfFileName].IsBool())
            return std::string(kOneNewlineAtEndOfFileName) + " not a boolean value.";
        oneNewlineAtEndOfFile = document[kOneNewlineAtEndOfFileName].GetBool();
    }

    if (document.HasMember(kNoMethodReturnWithLexicalScopeName)) {
        if (!document[kNoMethodReturnWithLexicalScopeName].IsBool())
            return std::string(kNoMethodReturnWithLexicalScopeName) + " not a boolean value.";
        noMethodReturnWithLexicalScope = document[kNoMethodReturnWithLexicalScopeName].GetBool();
    }

    if (document.HasMember(kWarnOnCurryArgumentName)) {
        if (!document[kWarnOnCurryArgumentName].IsBool())
            return std::string(kWarnOnCurryArgumentName) + " not a boolean value.";
        warnOnCurryArgument = document[kWarnOnCurryArgumentName].GetBool();
    }

    if (document.HasMember(kLintTestName)) {
        if (!document[kLintTestName].IsBool())
            return std::string(kLintTestName) + " not a boolean value.";
        lintTest = document[kLintTestName].GetBool();
    }

    return "";
}

std::string Config::writeJSON() const {
    rapidjson::Document document;
    document.SetObject();
    auto& alloc = document.GetAllocator();

    rapidjson::Value oneNewlineAtEndOfFileString;
    oneNewlineAtEndOfFileString.SetString(kOneNewlineAtEndOfFileName, alloc);
    document.AddMember(oneNewlineAtEndOfFileString, rapidjson::Value(oneNewlineAtEndOfFile), alloc);


    rapidjson::Value noMethodReturnWithLexicalScopeString;
    noMethodReturnWithLexicalScopeString.SetString(kNoMethodReturnWithLexicalScopeName, alloc);
    document.AddMember(noMethodReturnWithLexicalScopeString, rapidjson::Value(noMethodReturnWithLexicalScope), alloc);

    rapidjson::Value warnOnCurryArgumentString;
    warnOnCurryArgumentString.SetString(kWarnOnCurryArgumentName, alloc);
    document.AddMember(warnOnCurryArgumentString, rapidjson::Value(warnOnCurryArgument), alloc);

    rapidjson::Value noLintTestString;
    noLintTestString.SetString(kLintTestName, alloc);
    document.AddMember(noLintTestString, rapidjson::Value(lintTest), alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

} // namespace lint
