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

    if (document.HasMember(kNoCarriageReturnsInFile)) {
        if (!document[kNoCarriageReturnsInFile].IsBool())
            return std::string(kNoCarriageReturnsInFile) + " not a boolean value.";
        noCarriageReturnsInFile = document[kNoCarriageReturnsInFile].GetBool();
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

    rapidjson::Value noCarriageReturnsInFileString;
    noCarriageReturnsInFileString.SetString(kNoCarriageReturnsInFile, alloc);
    document.AddMember(noCarriageReturnsInFileString, rapidjson::Value(noCarriageReturnsInFile), alloc);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

} // namespace lint