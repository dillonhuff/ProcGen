#include "catch.hpp"

#include <fstream>

#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>

#include "llvm/ADT/STLExtras.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"

#include <iostream>

using namespace llvm;
using namespace std;

LLVMContext context;

namespace ProcGen {

  class FSMState {
  };

  class FSMCondition {
  };

  class FSMEdge {
  };

  typedef int NodeId;

  class LowFSM {

    NodeId nextNode;
    
    std::map<NodeId, FSMState> states;
    std::map<NodeId, std::vector<FSMEdge> > receivers;

  public:

    LowFSM() : nextNode(1) {}

    NodeId addState(const FSMState& st) {
      NodeId id = nextNode;
      nextNode++;
      states.insert({id, st});

      return id;
    }

    void setStartState(const NodeId id) {
      assert(contains_key(id, states));
      
    }

    void addEdge(const NodeId src, const NodeId dst, const FSMCondition cond) {
      
    }

    std::set<NodeId> getNodeIds() const {
      std::set<NodeId> ids;
      for (auto s : states) {
        ids.insert(s.first);
      }
      return ids;
    }
  };

  void emitVerilog(const std::string& moduleName, const LowFSM& fsm) {
    ofstream out(moduleName + ".v");
    out << "module " << moduleName << "(input clk, input rst)" << endl;

    

    for (auto nodeId : fsm.getNodeIds()) {
      out << "\t// Code for state " << nodeId << endl;
    }

    out << "endmodule" << endl;
    
    out.close();
  }

  TEST_CASE("Parse a tiny C program") {
    SMDiagnostic Err;
    LLVMContext Context;

    string modFile = "tiny_test.ll";
    std::unique_ptr<Module> Mod(parseIRFile(modFile, Err, Context));
    if (!Mod) {
      outs() << "Error: No mod\n";
      assert(false);
      //Err.print(argv[0], errs());
      //return 1;
    }

    // Go over all named mdnodes in the module
    for (Module::const_named_metadata_iterator I = Mod->named_metadata_begin(),
           E = Mod->named_metadata_end();
         I != E; ++I) {
      outs() << "Found MDNode:\n";
      // These dumps only work with LLVM built with a special cmake flag enabling
      // dumps.
      // I->dump();

      for (unsigned i = 0, e = I->getNumOperands(); i != e; ++i) {
        Metadata *Op = I->getOperand(i);
        if (auto *N = dyn_cast<MDNode>(Op)) {
          outs() << "  Has MDNode operand:\n  ";
          // N->dump();
          outs() << "  " << N->getNumOperands() << " operands\n";
        }
      }
    }

    outs() << "--All functions\n";
    for (auto& f : Mod->functions()) {
      outs() << "\t" << f.getName() << "\n";
    }

    //Mod->print(llvm::errs(), nullptr);

    const SymbolTableList<GlobalVariable>& gt = Mod->getGlobalList();
      
    outs() << "# of globals = " << gt.size() << "\n";

    for (auto& g : gt) {
      if (g.hasName()) {
        outs() << "Name = " << g.getName() << "\n";
      } else {
        outs() << "Global with no name?\n";
      }
    }

    Function* f = Mod->getFunction("main");
    assert(f != nullptr);

    LowFSM programState;
    
    cout << "Basic blocks in main" << endl;
    for (auto& bb : f->getBasicBlockList()) {
      outs() << "----- BASIC BLOCK" << "\n";
      outs() << bb << "\n";
      outs() << "Terminator for this block" << "\n";

      auto termInst = bb.getTerminator();
      outs() << bb.getTerminator()->getOpcode() << "\n";
      if (BranchInst::classof(termInst)) {
        outs() << "\t\tIs a branch" << "\n";
      } else {
        outs() << "\t\tNOT branch" << "\n";
      }
    }

    emitVerilog("tiny_test", programState);

  }

}
