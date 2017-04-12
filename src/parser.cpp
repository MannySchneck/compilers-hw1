#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>
#include <memory>
#include <deque>

#include <L1.h>
#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/raw_string.hh>


namespace L1 {

        /*
         * Grammar rules from now on.
         */
        struct label:
                pegtl::seq<
                pegtl::one<':'>,
                pegtl::plus<
                        pegtl::sor<
                                pegtl::alpha,
                                pegtl::one< '_' >
                                >
                        >,
                pegtl::star<
                        pegtl::sor<
                                pegtl::alpha,
                                pegtl::one< '_' >,
                                pegtl::digit
                                >
                        >
                > {};

        struct function_name:
                label {};

        struct number:
                pegtl::seq<
                pegtl::opt<
                        pegtl::sor<
                                pegtl::one< '-' >,
                                pegtl::one< '+' >
                                >
                        >,
                pegtl::plus<
                        pegtl::digit
                        >
                >{};

        struct argument_number:
                number {};

        struct local_number:
                number {} ;

        struct comment:
                pegtl::disable<
                pegtl::one< ';' >,
                pegtl::until< pegtl::eolf >
                > {};

        struct seps:
                pegtl::star<
                pegtl::sor<
                        pegtl::ascii::space,
                        comment
                        >
                > {};

        struct L1_label_rule:
                label{};

        struct argument_reg :
                pegtl::sor<
                pegtl::string<'r','d','i'>,
                pegtl::string<'r','s','i'>,
                pegtl::string<'r','d','x'>,
                pegtl::string<'r','c','x'>,
                pegtl::string<'r','8'>,
                pegtl::string<'r','9'>
                >{};

        struct writable_reg :
                pegtl::sor<
                argument_reg,
                pegtl::string<'r','a','x'>,
                pegtl::string<'r','b','x'>,
                pegtl::string<'r','b','p'>,
                pegtl::string<'r','1','0'>,
                pegtl::string<'r','1','1'>,
                pegtl::string<'r','1','2'>,
                pegtl::string<'r','1','3'>,
                pegtl::string<'r','1','4'>,
                pegtl::string<'r','1','5'>
                >
        {};

        struct reg :
                pegtl::sor <
                writable_reg,
                pegtl::string<'r','s','p'>
                >{};

        struct source :
                pegtl::sor <
                reg,
                number,
                label
                >{};

        struct mem_offset :
                number{};

        struct L1_mem_ref :
                pegtl::seq<
                pegtl::one<'('>,
                seps,
                pegtl::string<'m','e','m'>,
                seps,
                reg,
                mem_offset,
                pegtl::one<')'>
                >{};

        struct Left_arrow :
                pegtl::string<'<','-'>
        {};

        struct L1_basic_store :
                pegtl::sor<
                pegtl::seq<

                        writable_reg,
                        seps,
                        Left_arrow,
                        seps,
                        source
                        >
                // seq<
                //         writable_reg,
                //         seps,
                //         left_arrow,
                //         L1_mem_ref
                //         >,
                // seq<
                //         mem_ref,
                //         seps,
                //         left_arrow,
                //         source
                //         >
                >{};

        struct L1_instruction :
                pegtl::sor<
                L1_basic_store //,
                // L1_binop,
                // L1_cjump,
                // L1_label,
                // L1_goto,
                // L1_return,
                // L1_call,
                // L1_runtime_call,
                // L1_monop,
                // L1_lea
                >{};

        struct L1_instruction_rule :
                pegtl::seq<
                seps,
                pegtl::if_must<
                        pegtl::one<'('>,
                        seps,
                        L1_instruction,
                        seps,
                        pegtl::one<')'>
                        >
                >{};

        struct L1_instructions_rule :
                pegtl::seq<
                seps,
                pegtl::plus<L1_instruction_rule>
                >{};

        struct L1_function_rule:
                pegtl::seq<
                seps,
                pegtl::if_must<
                        pegtl::one< '(' >,
                        function_name,
                        seps,
                        argument_number,
                        seps,
                        local_number,
                        seps,
                        L1_instructions_rule,
                        seps,
                        pegtl::one< ')' >,
                        seps
                        >
                > {};

        struct L1_functions_rule:
                pegtl::seq<
                seps,
                pegtl::plus< L1_function_rule >
                > {};

        struct entry_point_rule:
                pegtl::seq<
                seps,
                pegtl::one< '(' >,
                seps,
                label,
                seps,
                L1_functions_rule,
                seps,
                pegtl::one< ')' >,
                seps
                > { };

        struct grammar :
                pegtl::must<
                entry_point_rule
                > {};

        /*
         * Data structures required to parse
         */

        using L1_item_ptr = std::unique_ptr<L1::Translatable>;

        class L1_Instruction_Elems_Stack {

        public:
                void push(L1_item_ptr item){
                        instr_elements.push_back(std::move(item));
                }

                L1_item_ptr pop(){
                        auto tmp = std::move(instr_elements.back());
                        instr_elements.pop_back();
                        return tmp;
                }

                const L1_item_ptr& peek() const{
                        return instr_elements.back();
                }

        private:
                std::deque<L1_item_ptr> instr_elements;
        };

        /*
         * Actions attached to grammar rules.
         */
        template< typename Rule >
        struct action : pegtl::nothing< Rule > {};

        template<> struct action < label > {
                static void apply( const pegtl::input & in, L1::Program & p){
                        if (p.entryPointLabel.empty()){
                                p.entryPointLabel = in.string();
                        }
                }
        };

        template<> struct action < function_name > {
                static void apply( const pegtl::input & in, L1::Program & p){
                        std::unique_ptr<L1::Function> newF(new L1::Function());
                        newF->name = in.string();
                        p.functions.push_back(std::move(newF));
                }
        };


        template<> struct action < argument_number > {
                static void apply( const pegtl::input & in, L1::Program & p){
                        const auto& currentF = p.functions.back();
                        currentF->arguments = std::stoll(in.string());
                }
        };

        template<> struct action < local_number > {
                static void apply( const pegtl::input & in, L1::Program & p){
                        const auto& currentF = p.functions.back();
                        currentF->locals = std::stoll(in.string());
                }
        };

        template<> struct action <L1_basic_store> {
                static void apply(const pegtl::input & in, L1::Program & p){
                        L1_Instruction_Elems_Stack stack;
                        std::unique_ptr<L1::Binop> store();

                }
        };



        Program L1_parse_file (char *fileName){

                /*
                 * Check the grammar for some possible issues.
                 */
                pegtl::analyze< L1::grammar >();

                /*
                 * Parse.
                 */

                Program p;
                pegtl::file_parser(fileName).parse< L1::grammar, L1::action >(p);

                return p;
        }

} // L1
