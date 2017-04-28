#include <L2/AST/jump.h>

Inst_Posn Jump::find_target(std::vector<Inst_Ptr>::const_iterator begin,
                            std::vector<Inst_Ptr>::const_iterator end,
                            const L2_Label &target) const{
        return std::find_if(begin,
                            end,
                            [this, target](Inst_Ptr elt){
                                    if(auto lab_p = dynamic_cast<L2_Label*>(elt.get())){
                                            return *lab_p == target;
                                    } else{
                                            return false;
                                    }
                            });
}
