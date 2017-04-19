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
#include <sstream>

#include "L1.h"
#include "parser.h"
#include <pegtl.hh>
#include <pegtl/trace.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/raw_string.hh>


namespace L1 {

        template<typename T>
        using uptr = std::unique_ptr<T>;


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

        struct entry_point_label:
                label{};

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

        struct L1_inst_label:
                label{};

        struct L1_jmp_label:
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
                pegtl::string<'r','s','p'>
        {};

        struct immediate:
                number{};

        struct source :
                pegtl::sor <
                writable_reg,
                reg,
                immediate,
                L1_jmp_label
                >{};

        struct value_source:
                pegtl::sor<
                writable_reg,
                immediate,
                reg
                >{};

        struct mem_offset :
                number{};

        struct L1_mem_ref :
                pegtl::seq<
                pegtl::one<'('>,
                seps,
//                pegtl::if_must<
                        pegtl::string<'m','e','m'>,
                        seps,
                        pegtl::sor<
                                reg,
                                writable_reg
                                >,
                        seps,
                        mem_offset,
                        seps,
                        pegtl::one<')'>
                        //>
                >{};

        struct left_arrow :
                pegtl::string<'<','-'>
        {};

        struct L1_basic_store :
                pegtl::seq<
                pegtl::sor<L1_mem_ref,
                           writable_reg
                           >,
                seps,
                left_arrow,
                seps,
                pegtl::sor<
                        source,
                        L1_mem_ref
                        >
                >{};

        struct add:
                pegtl::string<'+','='>{};
        struct sub:
                pegtl::string<'-','='>{};
        struct mult:
                pegtl::string<'*','='>{};
        struct band:
                pegtl::string<'&','='>{};

        struct L1_aop :
                pegtl::seq<
                pegtl::sor<
                        writable_reg,
                        L1_mem_ref
                        >,
                seps,
                pegtl::sor<
                        add,
                        sub,
                        mult,
                        band
                        >,
                seps,
                pegtl::sor<
                        pegtl::sor<
                                source,
                                L1_mem_ref
                                >
                        >
                >{};

        struct rcx :
                pegtl::string<'r', 'c','x'>{};

        struct left_shift:
                pegtl::string<'<','<','='>{};

        struct right_shift:
                pegtl::string<'>','>','='>{};


        struct L1_sop :
                pegtl::seq<
                writable_reg,
                seps,
                pegtl::sor<
                        left_shift,
                        right_shift
                        >,
                seps,
                pegtl::sor<
                        rcx,
                        immediate
                        >
                >{};

        struct inc :
                pegtl::string<'+','+'>{};
        struct dec :
                pegtl::string<'-','-'>{};


        struct L1_monop :
                pegtl::seq<
                writable_reg,
                seps,
                pegtl::sor<inc,
                           dec
                           >
                >{};


        struct L1_goto :
                pegtl::seq<
                pegtl::string<'g','o','t','o'>,
                seps,
                L1_jmp_label
                >{};

        struct le :
                pegtl::string<'<'>{};
        struct leq :
                pegtl::string<'<','='>{};
        struct eq :
                pegtl::string<'='>{};


        struct L1_cmp :
                pegtl::sor<
                leq,
                eq,
                le
                >{};

        struct L1_cmp_store :
                pegtl::seq<
                writable_reg,
                seps,
                left_arrow,
                seps,
                value_source,
                seps,
                L1_cmp,
                seps,
                value_source
                >{};

        struct L1_cjump :
                pegtl::seq<
                pegtl::string<'c','j','u','m','p'>,
                seps,
                value_source,
                seps,
                pegtl::if_must<
                        L1_cmp,
                        seps,
                        value_source,
                        seps,
                        L1_jmp_label,
                        seps,
                        L1_jmp_label
                        >
                >{};

        struct call_target :
                pegtl::sor<
                L1_jmp_label,
                writable_reg>{};

        struct L1_call :
                pegtl::seq<
                pegtl::string<'c','a','l','l'>,
                seps,
                call_target,
                seps,
                immediate>{};

        struct print :
                pegtl::string<'p','r','i','n','t'>{};
        struct alloc :
                pegtl::string<'a','l','l','o','c','a','t','e'>{};
        struct array_error :
                pegtl::string<'a','r','r','a','y','-','e','r','r','o','r'>{};

        struct runtime_fun :
                pegtl::sor<
                print,
                alloc,
                array_error>{};

        struct L1_rt_call:
                pegtl::seq<
                pegtl::string<'c','a','l','l'>,
                seps,
                runtime_fun,
                seps,
                number>{};

        struct L1_lea :
                pegtl::seq<
                writable_reg,
                seps,
                pegtl::if_must<
                        pegtl::string<'@'>,
                        seps,
                        writable_reg,
                        seps,
                        writable_reg,
                        seps,
                        immediate
                        >
                >{};

        struct L1_return :
                pegtl_string_t("return"){};

        struct L1_instruction :
                pegtl::sor<
                L1_cmp_store,
                L1_basic_store,
                L1_aop,
                L1_sop,
                L1_monop,
                L1_cjump,
                L1_goto,
                L1_return,
                L1_call,
                L1_rt_call,
                L1_lea,
                L1_return
                >{};

        struct L1_instruction_rule :
                pegtl::seq<
                seps,
                pegtl::sor<
                        pegtl::seq<
                        pegtl::if_must<
                                pegtl::one<'('>,
                                seps,
                                L1_instruction,
                                seps,
                                pegtl::one<')'>
                                                               >
                                >,
                        L1_inst_label
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
                        pegtl::one<')'>,
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
                entry_point_label,
                seps,
                L1_functions_rule,
                seps,
                pegtl::one<')'>,
                seps
                > { };

        struct grammar :
                pegtl::must<
                entry_point_rule
                > {};

        // Globals... Fuck you PEGTL. Fuck You. Fuckfuckfuckfuck.
        L1_Parse_Stack the_stack;

        std::vector<Binop_Op> aop_stack;
        std::vector<Shop_Op> shop_stack;
        std::vector<Cmp_Op> cmp_stack;
        std::vector<Monop_Op> monop_stack;
        std::vector<Runtime_Fun> rf_stack;

        void push_instr_curf(Program &p, Instruction* i){
                const auto& currentF = p.functions.back();
                currentF->instructions.push_back(std::unique_ptr<Instruction>(i));
        }

        /*
         * Actions attached to grammar rules.
         */
        template< typename Rule >
        struct action : pegtl::nothing< Rule > {};

        template<> struct action <entry_point_label> {
                static void apply(const pegtl::input &in, Program &p){
                        p.entryPointLabel = in.string();
                }
        };


        template<> struct action < function_name > {
                static void apply(const pegtl::input &in, Program &p){
                        std::unique_ptr<L1::Function> newF(new L1::Function());
                        newF->name = in.string();
                        p.functions.push_back(std::move(newF));
                }
        };


        template<> struct action < argument_number > {
                static void apply(const pegtl::input &in, Program &p){
                        const auto& currentF = p.functions.back();
                        currentF->arguments = std::stoll(in.string());
                }
        };

        template<> struct action < local_number > {
                static void apply(const pegtl::input &in, Program &p){
                        const auto& currentF = p.functions.back();
                        currentF->locals = std::stoll(in.string());
                }
        };


///////////////////////////////////////////////////////////////////////////////
//                                Instructions                               //
///////////////////////////////////////////////////////////////////////////////
        template<> struct action <L1_basic_store> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto rhs = the_stack.tr_pop<Binop_Rhs>();
                        auto lhs = the_stack.tr_pop<Binop_Lhs>();

                        push_instr_curf(p, new Binop(Binop_Op::store,
                                                     std::move(lhs),
                                                     std::move(rhs)));
                }
        };

        template<> struct action <add> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        aop_stack.push_back(Binop_Op::add_Assign);
                }
        };

        template<> struct action <sub> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        aop_stack.push_back(Binop_Op::sub_Assign);
                }
        };
        template<> struct action <mult> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        aop_stack.push_back(Binop_Op::mult_Assign);
                }
        };
        template<> struct action <band> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        aop_stack.push_back(Binop_Op::bit_And_Assign);
                }
        };


        template<> struct action <L1_aop> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto rhs = the_stack.tr_pop<Binop_Rhs>();
                        auto lhs = the_stack.tr_pop<Binop_Lhs>();

                        push_instr_curf(p, new Binop(aop_stack.back(),
                                                     std::move(lhs),
                                                     std::move(rhs)));
                        aop_stack.pop_back();
                }
        };

        template<> struct action <left_shift> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        shop_stack.push_back(Shop_Op::left_Shift);
                }
        };

        template<> struct action <right_shift> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        shop_stack.push_back(Shop_Op::right_Shift);
                }
        };


        template<> struct action <L1_sop> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto shift = the_stack.tr_pop<Value_Source>();
                        auto target = the_stack.tr_pop<Writable_Reg>();

                        push_instr_curf(p, new Shop(shop_stack.back(),
                                                    std::move(target),
                                                    std::move(shift)));
                        shop_stack.pop_back();
                }
        };

        template<> struct action <inc> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        monop_stack.push_back(Monop_Op::inc);
                }
        };
        template<> struct action <dec> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        monop_stack.push_back(Monop_Op::dec);
                }
        };


        template<> struct action <L1_monop> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto target = the_stack.tr_pop<Writable_Reg>();

                        push_instr_curf(p, new Monop(monop_stack.back(),
                                                     std::move(target)));
                        monop_stack.pop_back();
                }
        };


        template<> struct action <L1_inst_label> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        push_instr_curf(p, new L1_Target_Label(in.string()));
                }
        };

        template<> struct action <L1_goto>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto label = the_stack.tr_pop<L1_Label>();
                        push_instr_curf(p, new Goto(std::move(label)));
                }
        };

        template<> struct action <eq> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        cmp_stack.push_back(Cmp_Op::equal);
                }
        };
        template<> struct action <leq> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        cmp_stack.push_back(Cmp_Op::less_Equal);
                }
        };
        template<> struct action <le> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        cmp_stack.push_back(Cmp_Op::less);
                }
        };


        template<> struct action <L1_cjump>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto f_target = *(the_stack.tr_pop<L1_Label>());
                        auto t_target = *(the_stack.tr_pop<L1_Label>());
                        auto rhs = the_stack.tr_pop<Value_Source>();
                        auto lhs = the_stack.tr_pop<Value_Source>();
                        push_instr_curf(p, new Cond_Jump(cmp_stack.back(),
                                                         std::move(lhs),
                                                         std::move(rhs),
                                                         t_target,
                                                         f_target));
                        cmp_stack.pop_back();
                }
        };


        template<> struct action <L1_cmp_store>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto rhs = the_stack.tr_pop<Value_Source>();
                        auto lhs = the_stack.tr_pop<Value_Source>();
                        auto target = the_stack.tr_pop<Writable_Reg>();
                        push_instr_curf(p, new Comparison_Store(cmp_stack.back(),
                                                                std::move(lhs),
                                                                std::move(rhs),
                                                                *target));
                        cmp_stack.pop_back();
                }
        };

        template<> struct action <L1_call>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto arg_count = the_stack.tr_pop<Integer_Literal>();
                        auto target = the_stack.tr_pop<Callable>();
                        push_instr_curf(p, new Call(std::move(target),
                                                    arg_count->value));
                }
        };

        template<> struct action <print> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        rf_stack.push_back(Runtime_Fun::print);
                }
        };
        template<> struct action <alloc> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        rf_stack.push_back(Runtime_Fun::alloc);
                }
        };
        template<> struct action <array_error> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        rf_stack.push_back(Runtime_Fun::array_Error);
                }
        };


        template<> struct action<L1_rt_call>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        push_instr_curf(p, new Runtime_Call(rf_stack.back()));
                        rf_stack.pop_back();
                }
        };

        template<> struct action<L1_lea>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto mult = the_stack.tr_pop<Integer_Literal>();
                        auto offset = the_stack.tr_pop<Writable_Reg>();
                        auto base = the_stack.tr_pop<Writable_Reg>();
                        auto target = the_stack.tr_pop<Writable_Reg>();

                        push_instr_curf(p, new LEA{*target, *base, *offset, mult->value});
                }
        };

        template<> struct action<L1_instruction_rule>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.NUKE();
                }
        };

        template<> struct action<L1_return>{
                static void apply( const pegtl::input & in, L1::Program & p){
                        push_instr_curf(p, new Return(p.functions.back()->stack_shift()));
                }
        };


///////////////////////////////////////////////////////////////////////////////
//                                    Data                                   //
///////////////////////////////////////////////////////////////////////////////

        template<> struct action <reg> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.push(TrPtr(new Reg(in.string())));
                }
        };


        template<> struct action <rcx> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.push(TrPtr(new Writable_Reg(in.string())));
                }
        };


        template<> struct action <writable_reg> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.push(TrPtr(new Writable_Reg(in.string())));
                }
        };


        template<> struct action <immediate> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.push(TrPtr(new Integer_Literal(in.string())));
                }
        };


        template<> struct action <mem_offset> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.push(TrPtr(new Integer_Literal(in.string())));
                }
        };


        template<> struct action <L1_mem_ref> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        auto off = the_stack.tr_pop<Integer_Literal>();
                        auto base = the_stack.tr_pop<Reg>();

                        the_stack.push(TrPtr(new Memory_Ref(*base, off->value)));
                }
        };

        template<> struct action <L1_jmp_label> {
                static void apply( const pegtl::input & in, L1::Program & p){
                        the_stack.push(TrPtr(new L1_Label(in.string())));
                }
        };



        Program L1_parse_file (std::string fileName){

                /*
                 * Check the grammar for some possible issues.
                 */
                pegtl::analyze< L1::grammar >();

                /*
                 * Parse.
                 */

                Program p;
                pegtl::file_parser(fileName).parse< L1::grammar, L1::action>(p);

                return p;
        }

} // L1
