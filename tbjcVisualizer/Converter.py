import ROOT
import numpy as np

f = ROOT.TFile("output.root")
tree = f.Get("tree")

ROOT.gROOT.ProcessLine(
"struct tbjcPad\
{\
    int EventNum;\
    int PadNum;\
    float PadX;\
    float PadY;\
    int IsValid;\
    int RawAdc[512];\
    int MaxAdcIdx;\
    int IsPedestalSubtracted;\
    float Adc[512];\
    int IsAux;\
};" )
ROOT.gROOT.ProcessLine(
"struct tbjcPSA\
{\
    int EventNum;\
    float mesh[512];\
    float RhoVariance;\
    float QEventTot;\
};" )
ROOT.gROOT.ProcessLine(
"struct tbjcHit\
{\
    int EventNum;\
    int PadNum;\
    int hitNum;\
    float x;\
    float y;\
    float z;\
    float charge;\
};" )

tbjcPadReg = ROOT.tbjcPad()
tbjcPSAReg = ROOT.tbjcPSA()
tbjcHitReg = ROOT.tbjcHit()

tb0 = tree.GetBranch("Pad")
tb1 = tree.GetBranch("PSA")
tb2 = tree.GetBranch("Hit")

tb0.SetAddress(ROOT.AddressOf(tbjcPadReg,'EventNum'))
tb1.SetAddress(ROOT.AddressOf(tbjcPSAReg,'EventNum'))
tb2.SetAddress(ROOT.AddressOf(tbjcHitReg,'EventNum'))

tb1.GetEntry(12)
mesh = []
for _ in range(512):
    mesh.append(tbjcPSAReg.mesh[_])

print mesh

tb1.GetEntry(13)
mesh = []
for _ in range(512):
    mesh.append(tbjcPSAReg.mesh[_])

print mesh
