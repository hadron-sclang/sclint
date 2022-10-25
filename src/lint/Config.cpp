#include "Config.hpp"

#include "detectors/DetectorList.hpp"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace lint {

template <typename T> struct TypeList<T> {
    static void setDefaults(Config* config) { config->setOptionNamed(T::kOptionName, T::kDefaultValue); }

    static std::string readJSON(Config* config, rapidjson::Document& document) {
        if (document.HasMember(T::kOptionName)) {
            if (!document[T::kOptionName].IsBool()) {
                return std::string(T::kOptionName) + " not a boolean value.";
            }
            config->setOptionNamed(T::kOptionName, document[T::kOptionName].GetBool());
        }
        return "";
    }

    static void writeJSON(const Config* config, rapidjson::Document& document) {
        rapidjson::Value optionName;
        optionName.SetString(T::kOptionName, document.GetAllocator());
        document.AddMember(optionName, rapidjson::Value(config->getOptionNamed(T::kOptionName)),
                           document.GetAllocator());
    }
};

template <typename T, typename... Ts> struct TypeList {
    static void setDefaults(Config* config) {
        TypeList<T>::setDefaults(config);
        TypeList<Ts...>::setDefaults(config);
    }

    static std::string readJSON(Config* config, rapidjson::Document& document) {
        auto result = TypeList<T>::readJSON(config, document);
        if (result != "")
            return result;
        return TypeList<Ts...>::readJSON(config, document);
    }

    static void writeJSON(const Config* config, rapidjson::Document& document) {
        TypeList<T>::writeJSON(config, document);
        TypeList<Ts...>::writeJSON(config, document);
    }
};

void Config::initDefaults() { DetectorList::setDefaults(this); }

std::string Config::readJSON(std::string_view jsonString) {
    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(jsonString.data());

    // Validate some basic assumptions about the input JSON.
    if (!parseResult)
        return "failed to parse input JSON.";

    if (!document.IsObject())
        return "input JSON is not a dictionary.";

    return DetectorList::readJSON(this, document);
}

std::string Config::writeJSON() const {
    rapidjson::Document document;
    document.SetObject();

    DetectorList::writeJSON(this, document);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

bool Config::getOptionNamed(const std::string& name) const {
    auto optionIter = m_options.find(name);
    if (optionIter != m_options.end())
        return optionIter->second;
    assert(false && "Lookup of option failed.");
    return false;
}

void Config::setOptionNamed(const std::string& name, bool value) { m_options[name] = value; }

} // namespace lint
