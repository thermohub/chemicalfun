
HEADERS += \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser/ChemicalData.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser/ChemicalFormulaParser.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/FormulaParser/MoietyParser.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/Combiner.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/ChemicalReactions.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/Generator.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/MatrixUtils.h \
    $$CHEMICALFUN_HEADERS_DIR/ChemicalFun/ReactionsGenerator/Reaction.h \
    $$CHEMICALFUN_DIR/Common/Exception.h \
    $$CHEMICALFUN_DIR/ReactionsGenerator/CanonicalizerLeal16.h \
    $$CHEMICALFUN_DIR/ReactionsGenerator/GramSchmidtWeltin94.h \
    $$CHEMICALFUN_DIR/ReactionsGenerator/RowReduceSmithMissen98.h \
    $$CHEMICALFUN_DIR/FormulaParser/ElementsDB.h

SOURCES += \
    $$CHEMICALFUN_DIR/Common/Exception.cpp \
    $$CHEMICALFUN_DIR/FormulaParser/ChemicalData.cpp \
    $$CHEMICALFUN_DIR/FormulaParser/ChemicalFormulaParser.cpp \
    $$CHEMICALFUN_DIR/FormulaParser/MoietyParser.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/CanonicalizerLeal16.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/Combiner.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/ChemicalReactions.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/Generator.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/GramSchmidtWeltin94.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/MatrixUtils.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/RowReduceSmithMissen98.cpp \
    $$CHEMICALFUN_DIR/ReactionsGenerator/Reaction.cpp



