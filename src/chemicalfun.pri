
HEADERS += \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser/formuladata.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser/formulaparser.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/Combiner.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/DatabaseGenerator.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/Generator.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/MatrixUtils.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/ReactionGen.h \
    $$CHEMICALFUN_DIR/Common/Exception.h \
    $$CHEMICALFUN_DIR/ReactionsGenerator/CanonicalizerLeal16.h \
    $$CHEMICALFUN_DIR/ReactionsGenerator/GramSchmidtWeltin94.h \
    $$CHEMICALFUN_DIR/ReactionsGenerator/RowReduceSmithMissen98.h

SOURCES += \
    $$CHEMICALFUN_DIR/Common/Exception.cpp \
    $$CHEMICALFUN_DIR/FormulaParser/formuladata.cpp \
    $$CHEMICALFUN_DIR/FormulaParser/formulaparser.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/CanonicalizerLeal16.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/Combiner.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/DatabaseGenerator.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/Generator.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/GramSchmidtWeltin94.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/MatrixUtils.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/ReactionGen.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/RowReduceSmithMissen98.cpp



