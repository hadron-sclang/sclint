#include "Config.hpp"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace lint {

std::string Config::readJSON(std::string_view jsonString) {
    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(jsonString.data());

    // Validate some basic assumptions about the input JSON.
    if (!parseResult)
        return "failed to parse input JSON.";

    if (!document.IsObject())
        return "input JSON is not a dictionary.";

    if (document.HasMember("oneNewlineAtEndOfFile")) {
        if (!document["oneNewlineAtEndOfFile"].IsBool())
            return "oneNewlineAtEndOfFile not a boolean value.";
        oneNewlineAtEndOfFile = document["oneNewlineAtEndOfFile"].GetBool();
    }

    if (document.HasMember("noMethodReturnWithLexicalScope")) {
        if (!document["noMethodReturnWithLexicalScope"].IsBool())
            return "noMethodReturnWithLexicalScope not a boolean value.";
        noMethodReturnWithLexicalScope = document["noMethodReturnWithLexicalScope"].GetBool();
    }

    return "";
}

std::string Config::writeJSON() const {
    rapidjson::Document document;
    document.SetObject();
    auto& alloc = document.GetAllocator();

    document.AddMember("oneNewlineAtEndOfFile", rapidjson::Value(oneNewlineAtEndOfFile), alloc);
    document.AddMember("noMethodReturnWithLexicalScope", rapidjson::Value(noMethodReturnWithLexicalScope), alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

} // namespace lint