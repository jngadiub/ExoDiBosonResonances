suffixText=('#############################',
            '###                       ###',
            '###  SYSTEMATICS STUDIES  ###',
            '###                       ###',
            '#############################',
            '',
            '# This can be added to EDBR_main_cfg.py',
            '# to make the systematics studies. First,',
            '# we need to male some changes to the two paths',
            '# which make the final candidates',
            '',
            '# 1) The selected candidates now can be only from BestCandSelector (i.e, SIGNAL region), not from BestSidebandSelector',
            '',
            'process.allSelectedEDBRMu.src = cms.VInputTag(cms.InputTag("BestCandSelectorMu","singleJet"),',
            '                                              cms.InputTag("BestCandSelectorMu","doubleJet")',
            '                                              )',
            '',
            'process.allSelectedEDBREle.src = cms.VInputTag(cms.InputTag("BestCandSelectorEle","singleJet"),',
            '                                               cms.InputTag("BestCandSelectorEle","doubleJet")',
            '                                               )',
            '',
            '# 2) Create the modules which switch PDFs and create event weights.',
            '# According to http://www.hep.ucl.ac.uk/pdf4lhc/PDF4LHCrecom.pdf',
            '# we should use CTEQ66, MSTW2008 and NNPDF2.0.',
            '# I think it is ok to use NNPDF2.1, however...',
            '',
            'process.pdfWeights = cms.EDProducer("PdfWeightProducer",',
            '                                    # Fix POWHEG if buggy (this PDF set will also appear on output,',
            '                                    # so only two more PDF sets can be added in PdfSetNames if not "")',
            '                                    #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),',
            '                                    #GenTag = cms.untracked.InputTag("genParticles"),',
            '                                    PdfInfoTag = cms.untracked.InputTag("generator"),',
            '                                    PdfSetNames = cms.untracked.vstring("cteq66.LHgrid",',
            '                                                                        "MSTW2008lo68cl.LHgrid",',
            '                                                                        "NNPDF21_100.LHgrid"',
            '                                                                        )',
            '                                    )',
            '',
            '# 3) Add the pdf weight producers at the beginning of the paths.',
            'process.cmgEDBRZZMu.replace(process.badEventFilter,process.pdfWeights+process.badEventFilter)',
            'process.cmgEDBRZZEle.replace(process.badEventFilter,process.pdfWeights+process.badEventFilter)',
            '',
            '# 4) Create the modules which will calculate the changes when you move',
            '# from one PDF to the other.',
            'process.pdfSystMuChannel = cms.EDFilter("PdfSystematicsAnalyzer",',
            '                                        SelectorPath = cms.untracked.string("cmgEDBRZZMu"),',
            '                                        PdfWeightTags = cms.untracked.VInputTag("pdfWeights:cteq66",',
            '                                                                                "pdfWeights:MSTW2008lo68cl",',
            '                                                                                "pdfWeights:NNPDF21"',
            '                                                                                )',
            '                                        )',
            '',
            'process.pdfSystEleChannel = cms.EDFilter("PdfSystematicsAnalyzer",',
            '                                         SelectorPath = cms.untracked.string("cmgEDBRZZEle"),',
            '                                         PdfWeightTags = cms.untracked.VInputTag("pdfWeights:cteq66",',
            '                                                                                 "pdfWeights:MSTW2008lo68cl",',
            '                                                                                 "pdfWeights:NNPDF21"',
            '                                                                                 )',
            '                                         )',
            '',
            '# 5) Put only those modules in the EndPath',
            'process.outpath = cms.EndPath(process.pdfSystEleChannel+process.pdfSystMuChannel)')

#for line in suffixText:
#    print line

originalFile = open('../prod/EDBR_main_cfg.py','r')
systematicsFile = open('./EDBR_PdfSystematics_cfg.py','w')

for line in originalFile:
    systematicsFile.write(line)

for line in suffixText:
    systematicsFile.write(line+'\n')

systematicsFile.close()
originalFile.close()

print "EDBR_PdfSystematics_cfg.py created from ../prod/EDBR_main_cfg.py"

