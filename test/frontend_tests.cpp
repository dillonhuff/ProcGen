#include "catch.hpp"

// #include "llvm/ADT/StringRef.h"
// #include "llvm/ExecutionEngine/ExecutionEngine.h"
// #include "llvm/ExecutionEngine/MCJIT.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IRReader/IRReader.h"
// #include "llvm/Support/MemoryBuffer.h"
// #include "llvm/Support/raw_os_ostream.h"

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
      outs() << "\tfound function\n";
    }

   Mod->print(llvm::errs(), nullptr);

    const SymbolTableList<GlobalVariable>& gt = Mod->getGlobalList();
      
    outs() << "# of globals = " << gt.size() << "\n";

    for (auto& g : gt) {
      if (g.hasName()) {
        outs() << "Name = " << g.getValueName() << "\n";
      } else {
        outs() << "Global with no name?\n";
      }
    }

  }

    // llvm::InitializeAllTargets();
    // llvm::InitializeAllTargetMCs();
    // llvm::InitializeAllAsmPrinters();
    // llvm::InitializeAllAsmParsers();    

    // //InitializeNativeTarget();
    // //LLVMInitializeNativeAsmPrinter();


    // SMDiagnostic error;
    // //Module *m = new llvm::Module("llvm-mod", context); //parseIRFile("tiny_test.ll", error, context).get();
    //auto buf = MemoryBuffer::getMemBuffer("tiny_test.bc");
    //auto me = parseBitcodeFile(*buf, context); //parseBitcodeFile("tiny_test.ll", error, context).get();
    // if (me) {
    //   auto& m = me.get();
    //   //cout << "source file name " << m->getSourceFileName() << endl;
    //   llvm::errs() << "Module = " << m->getModuleIdentifier() << endl;
    //   //cout << "--All functions" << endl;
    //   // for (auto& f : m->functions()) {
    //   //   cout << "\tfound function" << endl;
    //   // }

    //   //m->print(llvm::errs(), nullptr);

    //   // const SymbolTableList<GlobalVariable>& gt = m->getGlobalList();
      
    //   // cout << "# of globals = " << gt.size() << endl;

    //   // for (auto& g : gt) {
    //   //   if (g.hasName()) {
    //   //     cout << "Name = " << g.getValueName() << endl;
    //   //   } else {
    //   //     cout << "Global with no name?" << endl;
    //   //   }
    //   //      }
    // } else {
    //   cout << "ERROR" << endl;
    // }

  //  }

}
