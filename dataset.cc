#include <iostream>
#include "TH1.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Pythia8/Pythia.h"
#include <TFile.h>
#include <TTree.h>
//#include "Pythia8Plugins/FastJet3.h"
#include <vector>
using namespace Pythia8;
int main()
{	
	FILE *fp;
	fp = fopen("jet_dataset.txt", "w");

	//TTree
	TFile *output = new TFile("dataset.root", "recreate");
	//(object, title)
	TTree *tree_dataset = new TTree("tree_dataset", "tree_dataset");

	int nListJets = 5;
	int nevents = 6000;

	// No event record printout.


	//nconst,pTD,Lesub 
	Int_t nconst_tree;
	Int_t qorg_tree;
	Double_t pTD_tree;
	Double_t LeSub_tree;
	Double_t g_tree;

	tree_dataset -> Branch("nconst", &nconst_tree);
	tree_dataset -> Branch("pTD", &pTD_tree);
	tree_dataset -> Branch("LeSub", &LeSub_tree);
	tree_dataset -> Branch("partonID",&qorg_tree);
	tree_dataset -> Branch("g", &g_tree);


	// Generator. LHC process and output selection. Initialization.
	Pythia pythia;
	pythia.readString("Beams:eCM = 13000.");
	pythia.readString("HardQCD:all = on");
	//カットをかけている
	pythia.readString("PhaseSpace:pTHatMin = 200.");

	pythia.readString("Next:numberShowInfo = 0");
	pythia.readString("Next:numberShowProcess = 0");
	pythia.readString("Next:numberShowEvent = 0");

	pythia.readString("Next:showMothersAndDaughters = on");
	pythia.readString("Random:setSeed = on");
	pythia.readString("Random:seed = 0");

	//乱数seed
	/*string iseed= argv[1];
	string  dummy="Random:seed = "+ iseed;
	//cout<< dummy <<endl;
	pythia.readString(dummy);*/

	pythia.init();



	// Common parameters for the two jet finders.
	double etaMax   = 4.;
	double radius   = 0.7;
	double pTjetMin = 10.;
	// Exclude neutrinos (and other invisible) from study.
	int    nSel     = 2;
	// Range and granularity of CellJet jet finder.
	int    nEta     = 80;
	int    nPhi     = 64;
	// Set up SlowJet jet finder, with anti-kT clustering
	// and pion mass assumed for non-photons..
	SlowJet slowJet( -1, radius, pTjetMin, etaMax, nSel, 1);

	// Set up CellJet jet finder.
	CellJet cellJet( etaMax, nEta, nPhi, nSel);
	// Histograms. Note similarity in names, even when the two jet finders
	// do not calculate identically the same property (pT vs. ET, y vs. eta).

	TH1F* eTgluonS = new TH1F("eTgluonS", "pT for gluon jets(>100 GeV), SlowJet", 200, 0., 500.);
	TH1F* eTquarkS = new TH1F("eTquarkS", "pT for quark jets(>100 GeV), SlowJet", 200, 0., 500.);
	TH1F* LeSubg = new TH1F("LeSubg", "LeSub for gluon jets(>100 GeV), SlowJet", 200, 0., 50.);
	TH1F* LeSubq = new TH1F("LeSubq", "LeSub for quark jets(>100 GeV), SlowJet", 200, 0., 50.);
	TH1F* pTDg = new TH1F("pTDg", "pTD for gluon jets(>100 GeV), SlowJet", 200, 0., 1.);
	TH1F* pTDq = new TH1F("pTDq", "pTD for quark jets(>100 GeV), SlowJet", 200, 0., 1.);
	TH1F* phiJetg = new TH1F("phiJetg", "phi for gjets, SlowJet", 100, -M_PI, M_PI);
	TH1F* phiJetq = new TH1F("phiJetq", "phi for qjets, SlowJet", 100, -M_PI, M_PI);
	TH1F* nconstg = new TH1F("nconstg", "nconst for gjets, SlowJet", 100, 0, 100);
	TH1F* nconstq = new TH1F("nconstq", "nconst for qjets, SlowJet", 100, 0, 100);
	TH1F* etag = new TH1F("etag", "eta for gjets, SlowJet", 100, -M_PI, M_PI);
	TH1F* etaq = new TH1F("etaq", "eta for qjets, SlowJet", 100, -M_PI, M_PI);
	TH1F* g_g = new TH1F("g_g", "g for gjets, SlowJet", 100, -0.5, 0.5);
	TH1F* g_q = new TH1F("g_q", "q for qjets, SlowJet", 100, -0.5, 0.5);
	//////////////////////////////////////////////////////////////// Begin event loop. Generate event. Skip if error.
	for(int i = 0; i < nevents; i++) {
		if(!pythia.next()) continue;
		// Analyze Slowet jet properties. List first few.
		slowJet. analyze( pythia.event );
		if (i < nListJets) slowJet.list();
		//ジェっとの情報のリストを作成する



		int iMother;
		//ここでトラックループを回すことができる。
		fprintf(fp, "========================event============================ \n");
		for (int i = 1; i < pythia.event.size(); ++i) {
			//id が100より小さくなるまでmotherを辿る
			//decay processで生成された粒子まで戻る
			iMother = i;	
			while( pythia.event[i].statusAbs() > 100)
				iMother = pythia.event[i].mother1();

			//if (pythia.event[iMother].id() == 21) {
			float mShift = pythia.event[i].m() - pythia.event[iMother].m();

			//}

			int entries = pythia.event.size();

		}

		//だいたい1イベント2000くらいのトラックができている。
		//cout << "---------------------------------------------------------" << endl;
		//cout << "event size :"<< pythia.event.size() << endl;


		//1eventで数個のジェットができると仮定し、全トラックのマザーがグルーオンであれば
		//そのeventで生成されたジェットはグルーオンジェットであると言える？？？？？
		//motherがgluon(ID = 21)だった場合のみジェットをfillする。

		int iMother_id = pythia.event[iMother].id();


		//cout << "number of jet :" << slowJet.sizeJet() << endl;
		fprintf(fp, "number of jets: %d \n", slowJet.sizeJet());

		//phiをpTの降順に並び替える
		vector<int> jetpT_down;
		vector<int> jetphi_down;
		int tmp = 0;

		for (int v = 0; v < slowJet.sizeJet(); ++v) {
			jetpT_down.push_back(slowJet.pT(v));
			jetphi_down.push_back(slowJet.phi(v));
		}

		//leading jet pt とsub-leading jet pt を設定

		for (int i = 0; i < slowJet.sizeJet(); ++i) {
			//leading particle
			int lp_n = 0;
			float lp_pt = 0;
			//sub leading particle
			int slp_n = 0;
			float slp_pt = 0;

			//pTD
			float pTD_d = 0, pTD_2= 0, pTD = 0;
			//g
			double g = 0, dR = 0, deta = 0, dphi = 0;

			fprintf(fp, "-------------------JET < %d >-----------------------  \n", i + 1);

			fprintf(fp, "number of jet particles : %d \n", slowJet.constituents(i).size());
			fprintf(fp, "particle number | pT:  \n");




			tmp = 0;
			//LeSubを取得するために降順のjetparticle list を作成する。
			//slowJet.constituentsのvectorをうまく並び替えることができなかったので、新しくvectorを定義し、値を入れ直した後並び替えている。
			vector<int> jetparticle_down;
			for (int v = 0; v < slowJet.constituents(i).size(); ++v) {
				jetparticle_down.push_back(slowJet.constituents(i)[v]);
			}



			for (int j = 0; j < slowJet.constituents(i).size(); ++j) {
				fprintf(fp, "%d             %f \n", slowJet.constituents(i)[j], pythia.event[slowJet.constituents(i)[j]].pT());

				//この条件式では単純にpT()が最大となるnumberを取得する。

				if (lp_pt < pythia.event[slowJet.constituents(i)[j]].pT() ) {
					lp_n = slowJet.constituents(i)[j];
					lp_pt = pythia.event[slowJet.constituents(i)[j]].pT();
				}

				for (int k=j+1; k< slowJet.constituents(i).size(); ++k) {
					if ( pythia.event[jetparticle_down[j]].pT() < pythia.event[jetparticle_down[k]].pT() )
					{
						tmp =  jetparticle_down[j];
						jetparticle_down[j] = jetparticle_down[k];
						jetparticle_down[k] = tmp;
					}
				}

				//gの計算
				//jetを仕分けした後に計算したほうが、早くなると思う。
				deta = slowJet.y(i) - pythia.event[slowJet.constituents(i)[j]].y();
				dphi = slowJet.phi(i) - pythia.event[slowJet.constituents(i)[j]].phi();
				dR = sqrt(deta * deta + dphi * dphi);
				g +=  pythia.event[slowJet.constituents(i)[j]].pT() * dR / slowJet.pT(i);


			}

			for (int t = 0; t < jetparticle_down.size(); ++t) {
				pTD_d += pythia.event[jetparticle_down[t]].pT();
				pTD_2 += pythia.event[jetparticle_down[t]].pT() * pythia.event[jetparticle_down[t]].pT();
			}


			pTD = sqrt(pTD_2) /pTD_d;

			float LeSub;

			LeSub = pythia.event[jetparticle_down[0]].pT() - pythia.event[jetparticle_down[1]].pT();

			fprintf(fp, "leading particle's Pt:      %f \n", pythia.event[jetparticle_down[0]].pT());
			fprintf(fp, "sub-leading particle's Pt:  %f \n", pythia.event[jetparticle_down[1]].pT());
			fprintf(fp, "LeSub: %f \n",LeSub );



			//何の粒子でもmotherまでたどれればそれでいい。
			int M = jetparticle_down[0];
			while( pythia.event[M].statusAbs() > 30 )
				M = pythia.event[M].mother1();

			//2212の時はdaughterを出力
			//大体2倍ぐらいデータ数が変わる
			int M_id = pythia.event[M].idAbs();
			//
			if (M_id == 2212) {
				M = pythia.event[M].daughter1();
			}
			M_id = pythia.event[M].idAbs();
			//

			//cout << "mother id of leading particle: " << M_id << endl;
			fprintf(fp, "mother id of leading particle: %d \n",M_id );
			//fprintf(fp, "----------------------------- \n");


			//ここでidによってジェットを仕分けしてヒストグラムを作成する。
			if (M_id == 21) {
				if (slowJet.pT(i) > 150 && fabs(slowJet.y(i)) < 1) {
					LeSubg -> Fill(LeSub);
					pTDg -> Fill(pTD);
					phiJetg ->Fill( slowJet.phi(i) );
					nconstg -> Fill(slowJet.constituents(i).size());
					etag -> Fill(slowJet.y(i));
					g_g -> Fill(g);


					//tree
					nconst_tree = slowJet.constituents(i).size();
					pTD_tree = pTD;
					LeSub_tree = LeSub;
					g_tree = g;
					//gjなので1
					qorg_tree = 1;
					tree_dataset -> Fill();


				}else {
					eTgluonS -> Fill(slowJet.pT(i));
				}
			}

			if (M_id < 9) {
				if (slowJet.pT(i) > 150 &&  fabs(slowJet.y(i)) < 1 ) {
					LeSubq -> Fill(LeSub);
					pTDq -> Fill(pTD);
					phiJetq ->Fill( slowJet.phi(i) );
					nconstq -> Fill(slowJet.constituents(i).size());
					etaq -> Fill(slowJet.y(i));
					g_q -> Fill(g);

					//tree
					nconst_tree = slowJet.constituents(i).size();
					pTD_tree = pTD;
					LeSub_tree = LeSub;
					g_tree = g;
					//qなので0
					qorg_tree = 0;
					tree_dataset -> Fill();
				} else {
					eTquarkS -> Fill(slowJet.pT(i));	
				}

			}

		}

	}    
	tree_dataset->Write("",TAxis::kWriteDelete);
	output->Write();
	output->Close();
	fclose(fp);
	return 0;
}
