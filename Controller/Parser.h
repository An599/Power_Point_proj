#pragma once
#include <string>
#include <vector>
#include <cctype>

namespace Controller {

    class Parser {
    public:
        enum class State {
            START,
            COMMAND,
            ARGUMENT,
            VALUE,
            COMMA,
            ERROR
        };

        struct ParseResult {
            bool syntaxValid;
            std::vector<std::string> tokens;
            std::string errorMessage;
        };

        ParseResult parse(const std::string& input) {
            ParseResult result;
            result.syntaxValid = true;

            if (input.empty()) {
                result.syntaxValid = false;
                result.errorMessage = "Empty input";
                return result;
            }

            std::vector<std::string> tokens = tokenize(input);

            if (tokens.empty()) {
                result.syntaxValid = false;
                result.errorMessage = "No tokens found";
                return result;
            }

            State currentState = State::START;

            for (size_t i = 0; i < tokens.size(); ++i) {
                const std::string& token = tokens[i];

                switch (currentState) {
                case State::START:
                    if (isValidCommand(token)) {
                        currentState = State::COMMAND;
                    }
                    else {
                        currentState = State::ERROR;
                        result.syntaxValid = false;
                        result.errorMessage = "Invalid command token";
                    }
                    break;

                case State::COMMAND:
                    currentState = State::ARGUMENT;
                    break;

                case State::ARGUMENT:
                    if (token[0] == '-') {
                        currentState = State::VALUE;
                    }
                    else if (token == ",") {
                        currentState = State::COMMA;
                    }
                    else {
                        currentState = State::ARGUMENT;
                    }
                    break;

                case State::VALUE:
                    currentState = State::ARGUMENT;
                    break;

                case State::COMMA:
                    currentState = State::VALUE;
                    break;

                case State::ERROR:
                    result.syntaxValid = false;
                    result.errorMessage = "Parse error in state machine";
                    break;
                }

                if (currentState == State::ERROR) {
                    break;
                }
            }

            result.syntaxValid = true;
            result.tokens = tokens;

            return result;
        }

    private:
        std::vector<std::string> tokenize(const std::string& input) {
            std::vector<std::string> tokens;
            std::string currentToken;
            bool inQuotes = false;

            for (size_t i = 0; i < input.length(); ++i) {
                char c = input[i];

                if (c == '"') {
                    inQuotes = !inQuotes;
                    continue;
                }

                if (std::isspace(c) && !inQuotes) {
                    if (!currentToken.empty()) {
                        tokens.push_back(currentToken);
                        currentToken.clear();
                    }
                }
                else {
                    currentToken += c;
                }
            }

            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
            }

            return tokens;
        }

        bool isValidCommand(const std::string& token) {
            if (token.empty()) return false;
            return std::isalpha(token[0]) || token[0] == '_';
        }
    };

}
