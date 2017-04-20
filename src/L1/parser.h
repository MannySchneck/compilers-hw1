#pragma once
#include <L1/AST/L1.h>
#include <deque>
#include <sstream>
#include <exception>
#include <string>

namespace L1{
        Program L1_parse_file (std::string fileName);

        class L1_Parse_Stack{

        public:
                void push(TrPtr item){
                        instr_elements.push_back(std::move(item));
                }

                template<typename T>
                std::unique_ptr<T> tr_pop(){
                        T* ptr;

                        TrPtr p = pop();
                        if((ptr = dynamic_cast<T*>(p.get()))){
                                p.release();
                                return std::unique_ptr<T>(ptr);
                        }
                        else {
                                std::cout << "WHAT THE FUCK" << std::endl;
                                std::stringstream ss;
                                ss << std::string("bad downcast to ") <<  typeid(T).name();
                                throw std::logic_error(ss.str());
                        }
                }

                TrPtr pop(){
                        if(instr_elements.empty()){
                                throw std::logic_error("tried to pop from empty instr stack!");
                        }
                        auto tmp = std::move(instr_elements.back());
                        instr_elements.pop_back();
                        return tmp;
                }

                const TrPtr& peek() const{
                        return instr_elements.back();
                }

                void NUKE(){
                        instr_elements.clear();
                }

        private:
                std::deque<TrPtr> instr_elements;
        };
}
