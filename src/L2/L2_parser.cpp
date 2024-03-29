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

#include <L2/AST/L2.h>
#include "L2_parser.h"
#include <pegtl.hh>
#include <pegtl/trace.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/raw_string.hh>

namespace L2 {

        template<typename T>
        using uptr = compiler_ptr<T>;


        /*
         * Grammar rules from now on.
         */

        struct L2_var_text :
                pegtl::seq<
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
                >{};

        struct L2_var :
                L2_var_text{};


        struct label:
                pegtl::if_must<
                pegtl::one<':'>,
                L2_var_text
                >
        {};

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

        struct L2_label_rule:
                label{};

        struct L2_inst_label:
                label{};

        struct L2_jmp_label:
                label{};

        struct shift_reg :
                pegtl::string<'r','c','x'>{};

        struct argument_reg :
                pegtl::sor<
                pegtl::string<'r','d','i'>,
                pegtl::string<'r','s','i'>,
                pegtl::string<'r','d','x'>,
                shift_reg,
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

        struct rsp :
                pegtl::string<'r','s','p'>{};

        struct writable :
                pegtl::sor<
                writable_reg,
                L2_var>{};


        struct reg :
                pegtl::sor<
                rsp,
                writable_reg
                >
        {};

        struct x :
                pegtl::sor<
                reg,
                writable
                >{};


        struct immediate:
                number{};

        struct shift_arg:
                pegtl::sor<
                immediate,
                shift_reg,
                L2_var>{};

        struct source :
                pegtl::sor <
                writable,
                reg,
                immediate,
                L2_jmp_label
                >{};

        struct value_source:
                pegtl::sor<
                writable,
                immediate,
                reg
                >{};

        struct mem_offset :
                number{};

        struct L2_mem_ref :
                pegtl::seq<
                pegtl::one<'('>,
                seps,
                pegtl::if_must<
                        pegtl::string<'m','e','m'>,
                        seps,
                        x,
                        seps,
                        mem_offset,
                        seps,
                        pegtl::one<')'>
                        >
                >{};

        struct left_arrow :
                pegtl::string<'<','-'>
        {};

        struct stack_arg :
                pegtl::seq<
                pegtl::one<'('>,
                seps,
                pegtl::if_must<
                pegtl_string_t("stack-arg"),
                seps,
                immediate
                        >,
                seps,
                pegtl::one<')'>
                >{};


        struct L2_basic_store :
                pegtl::seq<
                pegtl::sor<L2_mem_ref,
                           writable
                           >,
                seps,
                left_arrow,
                seps,
                pegtl::sor<
                        source,
                        L2_mem_ref,
                        stack_arg
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

        struct L2_aop :
                pegtl::seq<
                pegtl::sor<
                        writable,
                        L2_mem_ref
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
                                L2_mem_ref
                                >
                        >
                >{};

        struct rcx :
                pegtl::string<'r', 'c','x'>{};

        struct left_shift:
                pegtl::string<'<','<','='>{};

        struct right_shift:
                pegtl::string<'>','>','='>{};


        struct L2_sop :
                pegtl::seq<
                writable,
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


        struct L2_monop :
                pegtl::seq<
                writable,
                seps,
                pegtl::sor<inc,
                           dec
                           >
                >{};


        struct L2_goto :
                pegtl::seq<
                pegtl::string<'g','o','t','o'>,
                seps,
                L2_jmp_label
                >{};

        struct le :
                pegtl::string<'<'>{};
        struct leq :
                pegtl::string<'<','='>{};
        struct eq :
                pegtl::string<'='>{};


        struct L2_cmp :
                pegtl::sor<
                leq,
                eq,
                le
                >{};

        struct L2_cmp_store :
                pegtl::seq<
                writable,
                seps,
                left_arrow,
                seps,
                value_source,
                seps,
                L2_cmp,
                seps,
                value_source
                >{};

        struct L2_cjump :
                pegtl::seq<
                pegtl::string<'c','j','u','m','p'>,
                seps,
                value_source,
                seps,
                pegtl::if_must<
                        L2_cmp,
                        seps,
                        value_source,
                        seps,
                        L2_jmp_label,
                        seps,
                        L2_jmp_label
                        >
                >{};

        struct call_target :
                pegtl::sor<
                L2_jmp_label,
                writable>{};

        struct L2_call :
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

        struct L2_rt_call:
                pegtl::seq<
                pegtl::string<'c','a','l','l'>,
                seps,
                runtime_fun,
                seps,
                immediate>{};

        struct L2_lea :
                pegtl::seq<
                writable,
                seps,
                pegtl::if_must<
                        pegtl::string<'@'>,
                        seps,
                        writable,
                        seps,
                        writable,
                        seps,
                        immediate
                        >
                >{};

        struct L2_return :
                pegtl_string_t("return"){};

        struct L2_instruction :
                pegtl::sor<
                L2_cmp_store,
                L2_basic_store,
                L2_aop,
                L2_sop,
                L2_monop,
                L2_cjump,
                L2_goto,
                L2_return,
                L2_rt_call,
                L2_call,
                L2_lea,
                L2_return
                >{};

        struct L2_instruction_rule :
                pegtl::seq<
                seps,
                pegtl::sor<
                        pegtl::seq<
                                pegtl::if_must<
                                        pegtl::one<'('>,
                                        seps,
                                        L2_instruction,
                                        seps,
                                        pegtl::one<')'>
                                        >
                                >,
                        L2_inst_label
                        >
                >{};

        struct L2_instructions_rule :
                pegtl::seq<
                seps,
                pegtl::plus<L2_instruction_rule>
                >{};

        struct L2_function_rule:
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
                        L2_instructions_rule,
                        seps,
                        pegtl::one<')'>,
                        seps
                        >
                > {};

        struct L2_functions_rule:
                pegtl::seq<
                seps,
                pegtl::plus< L2_function_rule >
                > {};

        struct entry_point_rule:
                pegtl::seq<
                seps,
                pegtl::one< '(' >,
                seps,
                entry_point_label,
                seps,
                L2_functions_rule,
                seps,
                pegtl::one<')'>,
                seps
                > { };

        struct grammar :
                pegtl::must<
                entry_point_rule
                > {};

        // Globals... Fuck you PEGTL. Fuck You. Fuckfuckfuckfuck.
        L2_Parse_Stack<ASTPtr> the_stack;

        std::vector<Binop_Op> aop_stack;
        std::vector<Shop_Op> shop_stack;
        std::vector<Cmp_Op> cmp_stack;
        std::vector<Monop_Op> monop_stack;
        std::vector<Runtime_Fun> rf_stack;

        void push_instr_curf(Program &p, Instruction* i){
                const auto& currentF = p.functions.back();
                currentF->instructions.push_back(compiler_ptr<Instruction>(i));
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
                        compiler_ptr<L2::Function> newF(new L2::Function());
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
        template<> struct action <L2_basic_store> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto rhs = the_stack.downcast_pop<Binop_Rhs>();
                        auto lhs = the_stack.downcast_pop<Binop_Lhs>();

                        push_instr_curf(p, new Binop(Binop_Op::store,
                                                     std::move(lhs),
                                                     std::move(rhs)));
                }
        };

        template<> struct action <add> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        aop_stack.push_back(Binop_Op::add_Assign);
                }
        };

        template<> struct action <sub> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        aop_stack.push_back(Binop_Op::sub_Assign);
                }
        };
        template<> struct action <mult> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        aop_stack.push_back(Binop_Op::mult_Assign);
                }
        };
        template<> struct action <band> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        aop_stack.push_back(Binop_Op::bit_And_Assign);
                }
        };


        template<> struct action <L2_aop> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto rhs = the_stack.downcast_pop<Binop_Rhs>();
                        auto lhs = the_stack.downcast_pop<Binop_Lhs>();

                        push_instr_curf(p, new Binop(aop_stack.back(),
                                                     std::move(lhs),
                                                     std::move(rhs)));
                        aop_stack.pop_back();
                }
        };

        template<> struct action <left_shift> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        shop_stack.push_back(Shop_Op::left_Shift);
                }
        };

        template<> struct action <right_shift> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        shop_stack.push_back(Shop_Op::right_Shift);
                }
        };


        template<> struct action <L2_sop> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto shift = the_stack.downcast_pop<Value_Source>();
                        auto target = the_stack.downcast_pop<Writable>();

                        push_instr_curf(p, new Shop(shop_stack.back(),
                                                    std::move(target),
                                                    std::move(shift)));
                        shop_stack.pop_back();
                }
        };

        template<> struct action <inc> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        monop_stack.push_back(Monop_Op::inc);
                }
        };
        template<> struct action <dec> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        monop_stack.push_back(Monop_Op::dec);
                }
        };


        template<> struct action <L2_monop> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto target = the_stack.downcast_pop<Writable>();

                        push_instr_curf(p, new Monop(monop_stack.back(),
                                                     std::move(target)));
                        monop_stack.pop_back();
                }
        };


        template<> struct action <L2_inst_label> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        push_instr_curf(p, new L2_Target_Label(in.string()));
                }
        };

        template<> struct action <L2_goto>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto label = the_stack.downcast_pop<L2_Label>();
                        push_instr_curf(p, new Goto(std::move(label)));
                }
        };

        template<> struct action <eq> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        cmp_stack.push_back(Cmp_Op::equal);
                }
        };
        template<> struct action <leq> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        cmp_stack.push_back(Cmp_Op::less_Equal);
                }
        };
        template<> struct action <le> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        cmp_stack.push_back(Cmp_Op::less);
                }
        };


        template<> struct action <L2_cjump>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto f_target = *(the_stack.downcast_pop<L2_Label>());
                        auto t_target = *(the_stack.downcast_pop<L2_Label>());
                        auto rhs = the_stack.downcast_pop<Value_Source>();
                        auto lhs = the_stack.downcast_pop<Value_Source>();
                        push_instr_curf(p, new Cond_Jump(cmp_stack.back(),
                                                         std::move(lhs),
                                                         std::move(rhs),
                                                         t_target,
                                                         f_target));
                        cmp_stack.pop_back();
                }
        };


        template<> struct action <L2_cmp_store>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto rhs = the_stack.downcast_pop<Value_Source>();
                        auto lhs = the_stack.downcast_pop<Value_Source>();
                        auto target = the_stack.downcast_pop<Writable>();
                        push_instr_curf(p, new Comparison_Store(cmp_stack.back(),
                                                                std::move(lhs),
                                                                std::move(rhs),
                                                                target));
                        cmp_stack.pop_back();
                }
        };

        template<> struct action <L2_call>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto arg_count = the_stack.downcast_pop<Integer_Literal>();
                        auto target = the_stack.downcast_pop<Callable>();
                        push_instr_curf(p, new Call(std::move(target),
                                                    arg_count->value));
                }
        };

        template<> struct action <print> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        rf_stack.push_back(Runtime_Fun::print);
                }
        };
        template<> struct action <alloc> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        rf_stack.push_back(Runtime_Fun::alloc);
                }
        };
        template<> struct action <array_error> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        rf_stack.push_back(Runtime_Fun::array_Error);
                }
        };


        template<> struct action<L2_rt_call>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto numargs = the_stack.downcast_pop<Integer_Literal>();
                        push_instr_curf(p, new Runtime_Call(rf_stack.back(), numargs->value));
                        rf_stack.pop_back();
                }
        };

        template<> struct action<L2_lea>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto mult = the_stack.downcast_pop<Integer_Literal>();
                        auto offset = the_stack.downcast_pop<Writable>();
                        auto base = the_stack.downcast_pop<Writable>();
                        auto target = the_stack.downcast_pop<Writable>();

                        push_instr_curf(p, new LEA{target, base, offset, mult->value});
                }
        };

        template<> struct action<L2_instruction_rule>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.NUKE();
                }
        };

        template<> struct action<L2_return>{
                static void apply( const pegtl::input & in, L2::Program & p){
                        push_instr_curf(p, new Return(p.functions.back()->stack_shift()));
                }
        };


///////////////////////////////////////////////////////////////////////////////
//                                    Data                                   //
///////////////////////////////////////////////////////////////////////////////
        template<> struct action <rsp> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push(ASTPtr(new Reg(in.string())));
                }
        };

        template<> struct action <rcx> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push(ASTPtr(new Writable_Reg(in.string())));
                }
        };

        template<> struct action <writable_reg> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push(ASTPtr(new Writable_Reg(in.string())));
                }
        };

        template<> struct action <immediate> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push(ASTPtr(new Integer_Literal(in.string())));
                }
        };

        template<> struct action <mem_offset> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push(ASTPtr(new Integer_Literal(in.string())));
                }
        };


        template<> struct action <L2_mem_ref> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto off = the_stack.downcast_pop<Integer_Literal>();
                        auto base = the_stack.downcast_pop<Reg>();
                        the_stack.push(ASTPtr(new Memory_Ref(std::move(base), off->value)));
                }
        };

        template<> struct action <stack_arg> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        auto off = the_stack.downcast_pop<Integer_Literal>();
                        the_stack.push(ASTPtr(new Stack_Arg(off->value)));
                }
        };

        template<> struct action <L2_jmp_label> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push(ASTPtr(new L2_Label(in.string())));
                }
        };

        template<> struct action <L2_var> {
                static void apply( const pegtl::input & in, L2::Program & p){
                        the_stack.push( ASTPtr(new Var(in.string())));
                }
        };

        Program L2_parse_file (std::string fileName){

                /*
                 * Check the grammar for some possible issues.
                 */
                pegtl::analyze< L2::grammar >();

                /*
                 * Parse.
                 */

                Program p;
                pegtl::file_parser(fileName).parse< L2::grammar, L2::action>(p);

                return p;
        }

        Function parse_function_file (std::string fileName){

                /*
                 * Check the grammar for some possible issues.
                 */
                pegtl::analyze< L2::L2_function_rule>();

                /*
                 * Parse.
                 */

                Program p;
                pegtl::file_parser(fileName).parse< L2::L2_function_rule, L2::action>(p);

                return (std::move(*p.functions.back()));
        }


} // L2
