#pragma once
#ifndef VISEME_ANALYZER
#define VISEME_ANALYZER
#include<iostream>
#include<vector>
#include<deque>
#include<string>
#include<sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <map>
#include <fstream>

using namespace std;
typedef std::map<int, std::vector<string> > mymap;
typedef std::pair<int, std::vector<string> > mypair;
typedef std::map<string, std::vector<string> > mymap_s;
typedef std::pair<string, std::vector<string> > mypair_s;
enum enumViseme { P = 0, K, F, AE, AA, EE, AW, OO };
struct Visemes
{
	Visemes()//default constructor
	{
		vis = 0;
		freq = 1;
	}

	Visemes(int viseme_num, int fq)
	{
		vis = viseme_num;
		freq = fq;
	}
	int vis;

	int freq;

};

class VisemeAnalyzer
{
public:
	VisemeAnalyzer();
	//----------------Functions_-_-_-_-
	void resetBuffer();
	void addElementToLatestBuffer(int value);
	void noteInRecordPad();//assumes latest value has been set
	void noteInRecordPadForPrediction();
	void analyzeRecord();
	void showRecord();
	void clearRecord();
	void showRecordInConsole();
	void loadDictionary(string filename);
	void generateNewKeyOfWord();
	string generateNewKeyOfWord(std::vector<Visemes> rd);
	void shrinkKeyOfWord();
	//-------functions to create dictionary---------
	void setListOfWords();
	void addToBufferPad();
	void clearBufferPad();
	void chooseDataForStorage();
	bool getBufferStatus();
	void parseBufferDataToFile();
	void mergeTextChunks ( int num_of_text_files, string folder_location = RESOURCE_LOCATION_VA);
	bool crudeAnalysis();
	void pushLongKey();
	void storeLongKeys();
	string genLongKey();
	string genLongKey(const std::vector<Visemes> & rd);
	std::vector<Visemes> genRecordPad(string a_long_key);
	void storeSelectedLongKeys ();
	void cleanDataMembers_1();
	inline std::vector<string> reduceByOPR_2(  std::vector<Visemes> vect);
	inline std::vector<string> reduceByOPR (std::vector<Visemes> vect);
	void createTextChunk(const std::vector<string > & words_list, const std::vector<string > & related_keys, bool close_opstream);
	void createBigChunk();
	bool crudeAnalysis_OPR();
	inline std::vector<Visemes> reduceByIR(std::vector<Visemes> vect);
	inline std::vector<Visemes> reduceByIR(std::vector<Visemes> vect,int cut_off_wt);
	inline int rel_s(const int & vis_1, const int & vis_2);//relation strength
	inline void removeAE_K(std::vector<Visemes> & vect_vsm);
	inline std::vector<Visemes> removeAE_K(std::vector<Visemes> & vect_vsm,int anyvalue);
	void showVectViseme(const std::vector<Visemes> & vect_vsm);
	inline void refreshVectViseme(std::vector<Visemes> & vect_vsm);
	inline std::vector<int > groupVisemes_AB(const std::vector<Visemes> & vect_vsm);
	inline std::vector<int > groupVisemes_A(const std::vector<Visemes> & vect_vsm);
	inline int weightOf(Visemes x);
	inline std::vector<int> getIdxOfLeast(const std::vector<int> & wt_vector);
	inline std::vector<int> getIdxOfLeast(const std::vector<int> & wt_vector,int cut_off_wt);
	inline std::vector <Visemes> localReduceByRR(const std::vector<Visemes> & vsm_vect, int start, int end);
	inline bool findInVctr(int element, const std::vector<int> & vtr);
	inline std::vector<Visemes> reduceByRR(const std::vector<Visemes> & vis_vect,std::vector<int> partitions);
	inline std::vector<Visemes> compile3V(const std::vector<std::vector<Visemes> >& vvv);
	void createBigChunk_2 ();
	void createDictionary(string filename);
	std::vector<string > mergeTwoStringVectors(const std::vector<string > & vect1,  std::vector<string >  vect2);
	inline bool searchIn (std::vector<string> temp_anls_str);
	static string RESOURCE_LOCATION_VA;
	static const int RR_DATA[8][8];




	//-------functions to create dictionary---------
	//----------Data Members---------------------------
	static int BUFFER_SIZE;
	static int NUM_OF_VISEMES;
	static int K_threshold;
	static int AE_threshold;
	static int LENGTH_THRESHOLD;
	static int BUFFER_PAD_SIZE_LIMIT;

	int current_viseme;//direct access
	int scnd_last_viseme;
	
	string message;
	int flag;
	int predict_flag;
	bool end_point;
	string key_of_word;
	



	//-----test vars------------
	static int test_count;
	std::vector<string > list_of_words;
	std::vector<int> choices_to_store;


	//temp_test Vars
	std::vector<string> long_keys;//cleared by cdm_1;

	std::vector<Visemes> test_vv;
	Visemes test_vis;
	std::vector<Visemes> record_pad;
	//----------------const paras --------------------
	static int C_uplt;//handle with care
	static int B_uplt;
	static int A_uplt;



	//--------------------------------------
protected:
		
	cv::Mat predict_wm;
	cv::Mat predict_ori;
		std::vector<double> accuracy_vis;
		int buffer_size;
		std::deque<int> buffer;
		int record_pad_size;
		
		
		std::vector< std::vector<Visemes> > buffer_pad;
		cv::Mat message_mat;
		cv::Mat original_mat;
		stringstream ss;
		std::vector<string > viseme_cp_number;
		mymap dictionary;
		mymap_s dictionary_s;
		bool buffer_status_flag;// true: buffer_pad is full

	
};




//------------------------INLINES------------------------------

inline std::vector<string> VisemeAnalyzer::reduceByOPR_2(std::vector<Visemes> vect)
{
	std::vector<string> ret_keys(0);
	if (vect.size() > 0)
	{
		
		if (vect.size()>1)
		{
			//ret_keys.clear();
			ret_keys.push_back(VisemeAnalyzer::genLongKey(vect));
			int maxOPR_fq = vect[0].freq;
			for (int m = 0; m < (vect.size() - 1); m++)
			{
				if (maxOPR_fq < vect[m + 1].freq)
					maxOPR_fq = vect[m + 1].freq;
				cout << "Max:" << maxOPR_fq << "\n";
			}

			while (maxOPR_fq > 1)
			{
				cout << "IL_MAX_fq:" << maxOPR_fq << "\n";
				for (int k = 0; k < vect.size(); k++)
				{
					if (vect[k].freq == maxOPR_fq)
					{
						vect[k].freq -= 1;
					}
				}
				ret_keys.push_back(genLongKey(vect));
				maxOPR_fq -= 1;
			}//end of while 
		}
		else
		{
			for(int z=0;z<vect.size();z++)
			{
				if (vect[z].freq < 2)
				{
					vect[z].freq = 2;
				}
					while (vect[z].freq > 2)
					{

						ret_keys.push_back(genLongKey(vect));
						vect[z].freq -= 1;
					}
					ret_keys.push_back(genLongKey(vect));
				
			}

		}

	}
	
	
	else { ret_keys.push_back("9");
	cout << "reduced to zero size : err in OPR" << endl;
	}
	return ret_keys;
}

inline std::vector<string> VisemeAnalyzer::reduceByOPR(std::vector<Visemes> vect)
{
	std::vector<string> ret_keys(0);
	//ret_keys.clear();
	ret_keys.push_back(VisemeAnalyzer::genLongKey(vect));
	int maxOPR_fq = vect[0].freq;
	for (int m = 0; m < (vect.size() - 1); m++)
	{
		if (maxOPR_fq < vect[m + 1].freq)
			maxOPR_fq = vect[m + 1].freq;
		//cout << "Max:" << maxOPR_fq << "\n";
	}

	while (maxOPR_fq>1)
	{
		//cout << "IL_MAX_fq:" << maxOPR_fq << "\n";
		for (int k = 0; k < vect.size(); k++)
		{
			if (vect[k].freq == maxOPR_fq)
			{
				vect[k].freq -= 1;
			}
		}
		ret_keys.push_back(genLongKey(vect));
		maxOPR_fq -= 1;
	}//end of while

	return ret_keys;
}


inline std::vector<Visemes> VisemeAnalyzer::reduceByIR (std::vector<Visemes> vect)
{
	
	
	//std::vector<Visemes> ret_vv(0);
	if (vect.size() > 1)
	{
		std::vector <int > wt_vctr(0);
		for (int p = 0; p < vect.size(); ++p)
		{
			wt_vctr.push_back(weightOf(vect[p]));
		}
		std::vector<int> min_idx = getIdxOfLeast(wt_vctr);
		//cout << "in IR min_idx size=" << min_idx.size() << "  wt_vctr size=" << wt_vctr.size() << endl;
		for (int z = 0; z < min_idx.size(); ++z)
		{
			vect[min_idx[z]].vis = -1;
		}
		for (std::vector<Visemes>::iterator itr = vect.begin(); itr != vect.end(); ++itr)
		{
			if ((*itr).vis < 0)
			{
				vect.erase(itr);
				itr -= 1;
			}
		}
		refreshVectViseme(vect);
	
	}
	
	return vect;
	
}

inline std::vector<Visemes> VisemeAnalyzer::reduceByIR(std::vector<Visemes> vect,int cut_off_wt)
{

	if(vect.size()>1)
	//std::vector<Visemes> ret_vv(0);
	{std::vector <int > wt_vctr(0);
	for (int p = 0; p < vect.size(); ++p)
	{
		wt_vctr.push_back(weightOf(vect[p]));
	}
	std::vector<int> min_idx = getIdxOfLeast(wt_vctr, cut_off_wt);

	for (int z = 0; z < min_idx.size(); ++z)
	{
		vect[min_idx[z]].vis = -1;
	}
	for (std::vector<Visemes>::iterator itr = vect.begin(); itr != vect.end(); ++itr)
	{
		if ((*itr).vis < 0)
		{
			vect.erase(itr);
			itr -= 1;
		}
	}
	}
	return vect;

}



inline int VisemeAnalyzer::rel_s(const int & vis_1, const int & vis_2)
{
	if(RR_DATA[vis_1][vis_2] <= C_uplt  )
	{
		return 1;
	}
	else if (RR_DATA[vis_1][vis_2] <= B_uplt)
	{
		return 2;
	}
	else if (RR_DATA[vis_1][vis_2] <= A_uplt)
	{
		return 3;
	}
}


inline  void  VisemeAnalyzer::removeAE_K(std::vector<Visemes> & vect_vsm)
{
	vect_vsm.pop_back();//removes last two AEs or Ks
	/*int h_i = 0;
	while ((vect_vsm[h_i].vis == AE) || (vect_vsm[h_i].vis == K))
	{
		h_i += 1;
	}
	vect_vsm.erase(vect_vsm.begin(), vect_vsm.begin() + h_i);*/

	for (std::vector<Visemes> ::iterator itr = vect_vsm.begin(); itr != vect_vsm.end(); ++itr)
	{
		if ((*itr).vis == K)
		{
			vect_vsm.erase(itr);
			itr -= 1;//erasing reduces the size
		}
	}

	refreshVectViseme(vect_vsm);
	if (vect_vsm[0].vis == AE)
		vect_vsm.erase(vect_vsm.begin());

	

}

inline std::vector<Visemes> VisemeAnalyzer::removeAE_K(std::vector<Visemes> & vect_vsm, int anyvalue)
{

	vect_vsm.pop_back();

	for (std::vector<Visemes> ::iterator itr = vect_vsm.begin(); itr != vect_vsm.end(); ++itr)
	{
		if ((*itr).vis == K)
		{
			vect_vsm.erase(itr);
			itr -= 1;//erasing reduces the size
		}
	}

	refreshVectViseme(vect_vsm);
	if (vect_vsm[0].vis == AE)
		vect_vsm.erase(vect_vsm.begin());
	return vect_vsm;
}

inline void VisemeAnalyzer::refreshVectViseme(std::vector<Visemes> & vect_vsm)
{
	vect_vsm = genRecordPad(genLongKey(vect_vsm));
}


inline std::vector<int > VisemeAnalyzer::groupVisemes_AB(const std::vector<Visemes> & vect_vsm)
{//this function notes down the breakpoints when next viseme is  unrelated (level c) to the current viseme.
	std::vector<int > break_pts(0);
	for (int p = 0; p < (vect_vsm.size()-1); ++p)
	{
		if (rel_s(vect_vsm[p].vis, vect_vsm[p + 1].vis) <2)
			break_pts.push_back(p);

	}
	return break_pts;

}


inline std::vector<int > VisemeAnalyzer::groupVisemes_A(const std::vector<Visemes> & vect_vsm)
{
	std::vector<int > break_pts(0);
	for (int p = 0; p < (vect_vsm.size() - 1); ++p)
	{
		if (rel_s(vect_vsm[p].vis, vect_vsm[p + 1].vis) < 3)
			break_pts.push_back(p);

	}
	return break_pts;
}


inline int VisemeAnalyzer::weightOf(Visemes x)
{
	return(x.freq + RR_DATA[x.vis][x.vis]);
}

inline std::vector<int> VisemeAnalyzer::getIdxOfLeast(const std::vector<int> & wt_vector)
{
	std::vector<int> idx(0);
	int least_val = 0;
	if (wt_vector.size() > 0)
		least_val = wt_vector[0];
	for (int i = 0; i < wt_vector.size(); ++i)
	{
		if (wt_vector[i] <= least_val)
		{
			
			least_val = wt_vector[i];
		}
	}

	for (int g = 0; g < wt_vector.size(); ++g)
	{
		if (wt_vector[g] == least_val)
			idx.push_back(g);
	}
	
	return idx;
}

inline std::vector<int> VisemeAnalyzer::getIdxOfLeast(const std::vector<int> & wt_vector, int cut_off_wt)
{

	std::vector<int> idx(0);

	for (int i = 0; i < wt_vector.size(); ++i)
	{
		if (wt_vector[i] <=cut_off_wt)
		{
			idx.push_back(i);
			
		}
	}

	return idx;
}



inline std::vector <Visemes> VisemeAnalyzer::localReduceByRR(const std::vector<Visemes> & vsm_vect,int startidx,int endidx)//mind the indices
{
	if( (startidx<=endidx) && (endidx < vsm_vect.size()))
	{
		std::vector<Visemes > vect_vsm_ret(vsm_vect.begin() + startidx, vsm_vect.begin() + endidx + 1);
		if (startidx == endidx)
		{
			return vect_vsm_ret;
		}
		else
		{
			std::vector<int > rmv_idx(0);
			std::vector<int> wts(0);
			int max_rel = 0;
			int max_idx = 0;
			Visemes temp_vis(-1, -1);
			int lowest = 0;
			try
			{
				for (int h = startidx; h <= endidx; ++h)
				{
					wts.push_back(weightOf(vsm_vect[h]));
				}

				lowest = wts[0];

				for (int p = 0; p < wts.size(); p++)
				{
					if (wts[p] < lowest)
					{
						lowest = wts[p];
					}
				}

				for (int k = 0; k <= wts.size(); k++)
				{
					if (wts[k] == lowest)
					{
						rmv_idx.push_back(k);//k id for vect_vsm_ret // k+startidx for vsm_vect
					}

				}
				//if ALL are indexed
				if (rmv_idx.size() == wts.size())
				{
					int lwst = RR_DATA[vect_vsm_ret[0].vis][vect_vsm_ret[0].vis];
					for (int q = 0; q < vect_vsm_ret.size(); ++q)
					{
						if ((RR_DATA[vect_vsm_ret[q].vis][vect_vsm_ret[q].vis]) < lwst)
							lwst = RR_DATA[vect_vsm_ret[q].vis][vect_vsm_ret[q].vis];
					}

					for (int r = 0; r < (vect_vsm_ret.size() - 1); ++r)
					{
						if (RR_DATA[vect_vsm_ret[r].vis][vect_vsm_ret[r].vis] == lwst)
						{
							vect_vsm_ret[r].vis = -1;
							vect_vsm_ret[r + 1].freq += vect_vsm_ret[r].freq;
						}
					}

				}
				else
				{
					for (int m = 0; m < rmv_idx.size(); ++m)
					{
						for (int s = 0; s < vect_vsm_ret.size(); ++s)
						{
							if (!findInVctr(s, rmv_idx))
							{
								if (RR_DATA[vect_vsm_ret[rmv_idx[m]].vis][vect_vsm_ret[s].vis] >= max_rel)// max rel was initially 0(note: >=)
								{
									max_rel = RR_DATA[vect_vsm_ret[rmv_idx[m]].vis][vect_vsm_ret[s].vis];
									max_idx = s;//index of viseme which is  maximally related to the viseme at rmv_idx[m]
								}
							}

						}
						vect_vsm_ret[rmv_idx[m]].vis = -1;

						vect_vsm_ret[max_idx].freq = vect_vsm_ret[rmv_idx[m]].freq + vect_vsm_ret[max_idx].freq;
					}
				}//inner else ends
				for (std::vector<Visemes>::iterator itr = vect_vsm_ret.begin(); itr != vect_vsm_ret.end(); ++itr)
				{
					if ((*itr).vis < 0)
					{
						vect_vsm_ret.erase(itr);
						itr -= 1;
					}
				}

				//refreshVectViseme(vect_vsm_ret);
				return vect_vsm_ret;

			}
			catch (exception & exe)
			{
				cout << " Error in localReduceByRR." << endl;
				cout << exe.what() << endl;
			}

		}
		//else ends;
	}
	else
	{
		cout << "In localReduceByRR() endidx is out of range " << endl;
		return vsm_vect;
	}





}


inline bool VisemeAnalyzer::findInVctr(int element, const std::vector<int> & vtr)
{
	
	for (int k = 0; k < vtr.size(); ++k)
	{
		if (vtr[k] == element)
			return (true);
	}

	return (false);

}

inline std::vector<Visemes> VisemeAnalyzer::reduceByRR(const std::vector<Visemes> & vis_vect, std::vector<int> partitions)
{
	std::vector<std::vector<Visemes> > stack_vects(0);
	partitions.insert(partitions.begin(), -1);
	partitions.insert(partitions.end(), (vis_vect.size() - 1));

	for (int mk = 0; mk < (partitions.size()-1); ++mk)
	{
		stack_vects.push_back(localReduceByRR(vis_vect, partitions[mk]+1, partitions[mk + 1]));
	}


	return (compile3V(stack_vects));

}


inline std::vector<Visemes> VisemeAnalyzer::compile3V(const std::vector<std::vector<Visemes> >& vvv)//generates single vector<Visemes> from a vector<vector<Visemes> >
{
	std::vector<Visemes> ret_vv(0);
	for (int k = 0; k < vvv.size(); ++k)
	{
		for (int m = 0; m < vvv[k].size(); ++m)
		{
			ret_vv.push_back(vvv[k][m]);
		}
	}
	refreshVectViseme(ret_vv);
	return ret_vv;
}



inline bool VisemeAnalyzer::searchIn(std::vector<string> temp_anls_str)
{
	mymap_s::iterator itr;
	static int si_call_count = 0;
	
	for (int z = 0; z < temp_anls_str.size(); z++)
	{
		itr = dictionary_s.find(temp_anls_str[z]);

		if (itr != dictionary_s.end())
		{
			cout << "Prediction:\n";
			for (int i = 0; i < itr->second.size(); i++)
			{
				

				if ((si_call_count % 2) == 0)
				{
					cv::putText(predict_wm, itr->second.at(i), cv::Point(40 ,25*(i+1)),cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,255,255),2);
					cv::imshow("Prediction", predict_wm);
				}
				else
				{
					cv::putText(predict_wm, itr->second.at(i), cv::Point(40,25*(i+1)),cv::FONT_HERSHEY_PLAIN,2, cv::Scalar(255, 255, 0),2);
					cv::imshow("Prediction", predict_wm);
				}
				cout << itr->second.at(i) << endl;
			}
			
			predict_ori.copyTo(predict_wm);
			si_call_count += 1;
			return true;

		}
	}//for ends
	cv::putText(predict_wm,"..........", cv::Point(40, 25), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 255), 1);
	cv::imshow("Prediction", predict_wm);
	predict_ori.copyTo(predict_wm);
	return false;
}


#endif // !VISEME_ANALYZER