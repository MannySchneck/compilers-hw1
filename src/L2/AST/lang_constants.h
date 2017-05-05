#pragma once
#include <vector>
#include <string>
#include <unordered_set>

// TODO: Write a variadic template to make this happen
//class concat_vector

class Lang_Constants{
public:
        static const std::vector<std::string> arg_regs;
        static const std::vector<std::string> callee_saves;
        static const std::vector<std::string> caller_saves;
        static  std::vector<std::string> regs_vector;
        static std::unordered_set<std::string> regs_set;

        // Wierd RAII-ish hack
        static class _init {
        public:
                _init(){
                        regs_vector.insert(regs_vector.end(),callee_saves.begin(), callee_saves.end());
                        regs_set.insert(regs_vector.begin(), regs_vector.end());
                }
        } _initializer;
};
