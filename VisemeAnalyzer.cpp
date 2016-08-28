#include"VisemeAnalyzer.h"
#include<fstream>
//Change log:
//in noteInRecordPad addToBufferPad() is commented;
//in crude analysis return type changed to bool
int VisemeAnalyzer::BUFFER_SIZE = 2;
int VisemeAnalyzer::NUM_OF_VISEMES = 8;
int VisemeAnalyzer::K_threshold = 2;
int VisemeAnalyzer::AE_threshold = 2;
ofstream VA_stream ("C:/Users/shubham/Desktop/test/resources/VA_result.txt");
int VisemeAnalyzer::test_count = 0;
int VisemeAnalyzer::LENGTH_THRESHOLD = 4;
int VisemeAnalyzer::BUFFER_PAD_SIZE_LIMIT =11;
int VisemeAnalyzer::C_uplt = 3;//upper limit
int VisemeAnalyzer::B_uplt = 6;
int VisemeAnalyzer::A_uplt = 9;

string VisemeAnalyzer::RESOURCE_LOCATION_VA = "C:/Users/shubham/Desktop/test/resources/";

//GLOBALS


//____________________________RRDATA_______________________________________


const int VisemeAnalyzer::RR_DATA[8][8] =
    //0_P 1_K  2_F  3_AE  4_AA  5_EE  6_AW  7_OO
{
	{ 4,   2,   2,   3,     2,    2,    2,    1 },//0_P
	{ 2,   0,   3,   3,     4,    3,    3,    3 },//1_K
	{ 2,   3,   1,   5,     5,    3,    7,    3 },//2_F
	{ 3,   3,   5,   1,     5,    5,    3,    2 },//3_AE
	{ 2,   4,   5,   5,     1,    4,    5,    4 },//4_AA
	{ 2,   3,   3,   5,     4,    3,    2,    2 },//5_EE
	{ 2,   3,   7,   3,     5,    2,    3,    5 },//6_AW
	{ 1,   3,   3,   2,     4,    2,    5,    2 },//7_OO
};

//_________________________RRDATA__________________________________________



VisemeAnalyzer::VisemeAnalyzer() :message_mat(650, 100, CV_8UC1, cv::Scalar(0)), original_mat(650, 100, CV_8UC1, cv::Scalar(0)),
viseme_cp_number(NUM_OF_VISEMES, "OOP"), key_of_word("key_of_word"), list_of_words(1, "list_of_words"), choices_to_store(10, 0),
long_keys(1,"long_keys" ),test_vv(1,Visemes(1,1)),predict_wm(200,150,CV_8UC3,cv::Scalar(0,0,0)), predict_ori(200, 150, CV_8UC3, cv::Scalar(0, 0, 0))
{//_------------------------
	viseme_cp_number[0] = "P";
	viseme_cp_number[1] = "K";
	viseme_cp_number[2] = "F";
	viseme_cp_number[3] = "AE";
	viseme_cp_number[4] = "AA";
	viseme_cp_number[5] = "EE";
	viseme_cp_number[6] = "AW";
	viseme_cp_number[7] = "OO";


	//_----------------------
	message_mat.copyTo(original_mat);
	buffer_size = BUFFER_SIZE;
	accuracy_vis.resize(NUM_OF_VISEMES);
	try {
		accuracy_vis[0] = 1;
		accuracy_vis[1] = 1;
		accuracy_vis[2] = 1;
		accuracy_vis[3] = 1;
		accuracy_vis[4] = 1;
		accuracy_vis[5] = 1;
		accuracy_vis[6] = 1;
		accuracy_vis[7] = 1;

	}

	catch (exception & e)
	{
		cout << "Err in VisAnalyzer::  " << e.what() << endl;
	}

	record_pad.resize(1, Visemes(AE, 1));
	buffer_pad.resize(1,record_pad);
	resetBuffer();

	buffer_status_flag = false;
	predict_flag = 0;
	buffer_pad.clear();
	test_vv.clear();
	long_keys.clear();
	//cv::namedWindow("Prediction", 1);
	cv::imshow("Prediction", predict_wm);
}


void VisemeAnalyzer::resetBuffer()
{
	buffer.resize(buffer_size, AE);
}


void VisemeAnalyzer::addElementToLatestBuffer(int value)
{
	buffer.pop_back();
	buffer.push_front(value);
}

void VisemeAnalyzer::noteInRecordPad()
{
	
	if (buffer[0] == buffer[1])
		record_pad[record_pad.size() - 1].freq += 1;
	else
	{
		record_pad.push_back(Visemes(buffer[0], 1));
	}


	if (    (record_pad[record_pad.size() - 1].vis == AE  && record_pad[record_pad.size()-1].freq  >= AE_threshold)

		 || (record_pad[record_pad.size() - 1].vis == K  && record_pad[record_pad.size() - 1].freq >= K_threshold)         )//enum of aa
	{
		flag = 1;
		addToBufferPad();

	}



}


void VisemeAnalyzer::noteInRecordPadForPrediction ()
{

	if (buffer[0] == buffer[1])
		record_pad[record_pad.size() - 1].freq += 1;
	else
	{
		record_pad.push_back(Visemes(buffer[0], 1));
	}


	if ((record_pad[record_pad.size() - 1].vis == AE  && record_pad[record_pad.size() - 1].freq >= AE_threshold)

		|| (record_pad[record_pad.size() - 1].vis == K  && record_pad[record_pad.size() - 1].freq >= K_threshold))//enum of aa
	{
		flag = 1;
		if (record_pad.size() >= LENGTH_THRESHOLD)
		{
			predict_flag = 1;
			//addToBufferPad();
		}
	}



}



void VisemeAnalyzer::analyzeRecord()
{


}


void VisemeAnalyzer::showRecord()
{
	
	ss.clear();
	original_mat.copyTo(message_mat);
	ss << "Size=" << record_pad.size();
	ss >> message;
	if(message_mat.rows>50)
	cv::putText(message_mat, message, cv::Point(0,  50), cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(255), 1);
	
	for (int i = 0; i < record_pad.size(); i++)
	{
		ss.clear();
		ss << (record_pad.size() - i - 1);
		ss << "::";
		ss << record_pad[record_pad.size() - i].vis << "->";
		ss << record_pad[record_pad.size() - i].freq;
		ss >> message;
		if ((i + 2) * 50 < message_mat.rows)
			cv::putText(message_mat,message, cv::Point(0, (i + 2) * 50), cv::FONT_HERSHEY_PLAIN, 0.8, cv::Scalar(255), 1);

	}
	
	imshow("Record", message_mat);
	
}

void VisemeAnalyzer::clearRecord()
{
	record_pad.clear();
	record_pad.resize(1, Visemes(buffer[0], 1));
}

void VisemeAnalyzer::showRecordInConsole()
{
	try
	{

		test_count += 1;
		cout << "Word:" << test_count << endl;
		VA_stream << "Word:" << test_count << endl;
		for (int i = 0; i < record_pad.size(); i++)
		{

			try
			{
				cout << i << "_:  " << record_pad[i].vis <<"::"<< viseme_cp_number[record_pad[i].vis] << " fqc=" << record_pad[i].freq << endl;
				VA_stream << i << "_:  " << record_pad[i].vis << "::" << viseme_cp_number[record_pad[i].vis] << " fqc=" << record_pad[i].freq << endl;

			}
			catch (const std::exception& g)
			{
				cout << "In+In show Record" << g.what() << endl;
			}

		}

		cout << endl << endl;
		VA_stream << endl << endl;
	}
	catch (const std::exception&e)
	{
		cout << "IN showRecord" << endl;
	}

}

void VisemeAnalyzer::loadDictionary(string filename)
{
	try
	{
		ifstream input_stream(filename.c_str());
		stringstream tss;
		string temp;
		string temp_key_s;
		int temp_key;
		std::vector<string > temp_words(0);
		int noiid = 0;//number of items in the dictionary
		while (input_stream>>temp)
		{
			if (temp == "_key")
			{
				cout << temp << endl;
				noiid += 1;
				input_stream >> temp_key_s;
				tss.clear();
				tss<< temp_key_s;
				tss >> temp_key;
				cout << temp_key << endl;
				
			}
			else if (temp == "_over")
			{
				dictionary[temp_key] = temp_words;
				dictionary_s[temp_key_s] = temp_words;
				temp_words.clear();
			}
			else
			{
			
					temp_words.push_back(temp);
					cout << temp_words[temp_words.size() - 1] << endl;
				
			}


		}

	/*	for (mymap::iterator itr = dictionary.begin(); itr != dictionary.end(); ++itr)
		{
			cout << itr->first << endl;
			cout << " ---" << endl;
			for (int i = 0; i<itr->second.size(); i++)
				cout << itr->second[i] << endl;

			cout << "---------------------" << endl;
		}*/
		string loc_diction_save = RESOURCE_LOCATION_VA + "MAP_OF_DICTIONARY.txt";
		ofstream op_dict_save(loc_diction_save.c_str(), ios::out);
		cout << "-----------DICTIONARY_S-----------" << endl;
		for (mymap_s::iterator itr = dictionary_s.begin(); itr != dictionary_s.end(); ++itr)
		{
			cout << itr->first << endl;
			cout << " ---" << endl;
			for (int i = 0; i<itr->second.size(); i++)
				cout << itr->second[i] << endl;

			cout << "---------------------" << endl;
			//-------------------------------
			op_dict_save << itr->first << endl;
			op_dict_save << " ---" << endl;
			for (int i = 0; i<itr->second.size(); i++)
				op_dict_save << itr->second[i] << endl;

			op_dict_save << "---------------------" << endl;

			//-------------------------------
		}

		input_stream.close();
		op_dict_save.close();

	}
	catch (const std::exception&e)
	{
		cout << " ERR: " << e.what() << endl;
	}



}

void VisemeAnalyzer::generateNewKeyOfWord()
{
	ss.clear();
	//cout << "in generate" << endl;
	for (int i = 0; i < record_pad.size(); i++)
	{
		//cout << record_pad[i].vis << endl;
		ss << record_pad[i].vis;
	}
	ss >> key_of_word;
	cout << "KEY==" << key_of_word << endl;
}

string VisemeAnalyzer::generateNewKeyOfWord(std::vector<Visemes> rd)
{
	ss.clear();
	string key_of_word_temp="0";
	//cout << "in generate" << endl;
	for (int i = 0; i < rd.size(); i++)
	{
		//cout << record_pad[i].vis << endl;
		ss << rd[i].vis;
	}
	ss >> key_of_word_temp;
	std::cout << "KEY==" << key_of_word_temp << endl;
	return key_of_word_temp;
}

void VisemeAnalyzer::shrinkKeyOfWord() //don't clear recordPad before using it
{
	


}

void VisemeAnalyzer::setListOfWords()
{
	cout << "Enter Num Of Words" << endl;
	int num_of_wrd = 0;
	cin >> num_of_wrd;
	string set_word_temp="J";
	list_of_words.clear();
	int local_count = 0;

	for (int i = 0; i < num_of_wrd; i++)
	{
		cout << "Enter[" << i+1 << "]" << endl;
		cin.clear();
		cin.ignore(500,'\n');
		cin >> set_word_temp; cout << endl;
		if (set_word_temp == "N" || set_word_temp == "n")
		{
			cout << (i) << " words entered." << endl;
			break;
		}
		list_of_words.push_back(set_word_temp);
		local_count += 1;
	}


	cout << "Stored Words:" << endl;
	for (int i = 0; i < list_of_words.size(); i++)
	{
		cout << i << ":" << list_of_words[i] << endl;
	}
	cout << "------------------------" << endl;
}


void VisemeAnalyzer::addToBufferPad()
{
	if (  (record_pad.size() >= LENGTH_THRESHOLD ) && !buffer_status_flag )
	{
		
		buffer_pad.push_back(record_pad);
		this->pushLongKey();

		/*cout << "Pushed Back";
		for (int i = 0; i < record_pad.size(); i++)
			cout << i << " vis=" << record_pad[i].vis << " frq=" << record_pad[i].freq << endl;
		cout << "Pushed Back Showing Buffer\n" << "buffer_pad_size:" << buffer_pad.size() << endl;

		for (int i = 0; i < buffer_pad[buffer_pad.size()-1].size(); i++)
			cout << i << " vis=" << buffer_pad[buffer_pad.size() - 1][i].vis << " frq=" << buffer_pad[buffer_pad.size() - 1][i].freq << endl;*/
		if (buffer_pad.size() == BUFFER_PAD_SIZE_LIMIT)
		{
			buffer_status_flag = true;//true means FULL
			cout << "buffer_pad is full" << endl;
			
		}
	}
}


void VisemeAnalyzer::clearBufferPad()
{
	buffer_pad.clear();
	buffer_status_flag = false;
}

void VisemeAnalyzer::chooseDataForStorage()
{
	choices_to_store.clear();
	for (int i = 0; i < buffer_pad.size(); i++)
	{
		cout << "ElementNum=" << i << endl;
		cout << "BufferPad[" << i << "].size()= " << buffer_pad[i].size() << endl;
		for (int j = 0; j < buffer_pad[i].size(); j++)
		{
			
			try
			{
				cout << j << "_:" << buffer_pad[i][j].vis << "::"
					<< viseme_cp_number[buffer_pad[i].at(j).vis] << " fqc=" << buffer_pad[i].at(j).freq << endl;

			}
			catch (const std::exception& Exc)
			{
				cout << Exc.what() << endl;
			}
		}

		int select_or_not = 0;
		cout << "Press 1 for selecting and 0 for rejecting.  ";
		cin.ignore();
		cin.clear();
		cin >> select_or_not;
		if(select_or_not==1)
		{
			choices_to_store.push_back(i);
		}

	}
	cout << "\nYou Selected:\n";
	for (int k = 0; k < choices_to_store.size();)
	{
		cout << choices_to_store[k] << "  ";
		k++;
	}
	cout << endl;

}

bool VisemeAnalyzer::getBufferStatus()
{
	return buffer_status_flag;
}


void VisemeAnalyzer::parseBufferDataToFile()
{
	string filename_dict;
	cout << "Enter Filename(eg.PEN.txt)" << endl;
	cin.clear();
	cin >> filename_dict;
	filename_dict = RESOURCE_LOCATION_VA + filename_dict;
	ofstream op_dict_file (filename_dict.c_str(),ios::out);
	string tempo_key = "sk2";
	for (int i = 0; i < choices_to_store.size(); i++)
	{
		op_dict_file << "_key"<<"\n\n";
		tempo_key = generateNewKeyOfWord(buffer_pad[choices_to_store[i]]);
		op_dict_file << tempo_key<<"\n\n";
		for (int k = 0; k < list_of_words.size(); k++)
		{
			op_dict_file << list_of_words[k]<<"\n";
		}

		op_dict_file << "_over"<<"\n\n\n";
	}
	op_dict_file.close();

}

void VisemeAnalyzer::mergeTextChunks(int num_of_text_files, string folder_location)//[1,num_of_files]
{

	cout << "Enter File Name for Compiled Data(e.g. DICTIONARY.txt)" << endl;
	string dict_name = "DICTIONARY.txt";
	cin.clear();
	
	//cin.ignore(1, '\n');
	cin >> dict_name;
	string temp_loc_ = folder_location+dict_name;
	string temp_file_initials = "initials";
	cout << "Enter File Initials(e.g. test_ for test_1.txt or \0 for NULL.)" << endl;
	cin.clear();
	cin.ignore(500, '\n');
	cin >> temp_file_initials;
	string temp_dttf = "s";//data transfer
	ofstream output_dictionary (temp_loc_.c_str() ,ios::out);
	stringstream  mtc_ss;
	ifstream ip_fs(temp_loc_.c_str(), ios::in);
	for (int k = 1; k <= num_of_text_files; k++)
	{
		cout << k << endl;
		ip_fs.clear();
		ip_fs.close();
		mtc_ss.clear();
		mtc_ss << folder_location;
		mtc_ss << temp_file_initials<<k << ".txt";
		mtc_ss >> temp_loc_;
		cout << "T_loc:" << temp_loc_ << endl;
		ip_fs.open(temp_loc_.c_str(),ios::in);
		if (ip_fs.is_open())
		{
			while (ip_fs >> temp_dttf)
			{
				output_dictionary << temp_dttf << "\n";
			}
		}
		else cout << "File " << temp_loc_ << " can't be opened!" << endl;

		output_dictionary << "\n";
		
	}

	output_dictionary.close();
}

bool VisemeAnalyzer::crudeAnalysis()

{
	generateNewKeyOfWord();
	mymap_s::iterator itr = dictionary_s.find(key_of_word);
	if (itr != dictionary_s.end())
	{
		cout << "Guess:\n";
		for (int i = 0; i < itr->second.size(); i++)
		{
			cout << itr->second.at(i) << endl;
		}

		return true;

	}
	else
	{
		cout << "Unknown Word\n";
		return false;
	}
}

void VisemeAnalyzer::pushLongKey()
{

	long_keys.push_back(genLongKey());

	//long_keys.push_back(genLongKey( (genRecordPad(genLongKey(this->record_pad)))));
}


void VisemeAnalyzer::storeLongKeys()
{
	cout << "Enter filename To store long keys.(e.g. long_1.txt)" << endl;
	string tfn_SLK = "0";
	cin.ignore();
	cin >> tfn_SLK;
	tfn_SLK = RESOURCE_LOCATION_VA + tfn_SLK;

	ofstream SLK_ostream(tfn_SLK.c_str(), ios::out);
	if (SLK_ostream.is_open())
	{

		SLK_ostream << "_words\n\n";
		for (int k = 0; k < list_of_words.size(); k++)
		{
			SLK_ostream << list_of_words[k] << "\n";
		}
		SLK_ostream << "_wordsover\n\n";

		SLK_ostream << "_longkeys\n";

		for (int i = 0; i < long_keys.size(); i++)
		{
			SLK_ostream << long_keys[i] << "\n";

		}


		SLK_ostream << "_longkeysover\n";
		SLK_ostream.close();
		//long_keys.clear();

	}
	else {
		cout << "Err in store longkeys";
	}
}


string VisemeAnalyzer::genLongKey()
{
	string temp_long_key_plk = "0";
	ss.clear();
	//cout << "in generate" << endl;
	for (int i = 0; i < record_pad.size(); i++)
	{
		//cout << record_pad[i].vis << endl;
		for (int m = 0; m < record_pad[i].freq; m++)
		{
			ss << record_pad[i].vis;
		}
	}
	ss >> temp_long_key_plk;
	//cout << "LONG KEY_" << long_keys.size() << "= " << temp_long_key_plk << endl;
	return temp_long_key_plk;

}


string VisemeAnalyzer::genLongKey(const std::vector<Visemes> &  rd)
{
	string temp_long_key_plk = "0";
	ss.clear();
	//cout << "in generate" << endl;
	for (int i = 0; i < rd.size(); i++)
	{
		//cout << record_pad[i].vis << endl;
		for (int m = 0; m < rd[i].freq; m++)
		{
			ss << rd[i].vis;
		}
	}
	ss >> temp_long_key_plk;
	//cout << "LONG KEY_" << long_keys.size() << "= " << temp_long_key_plk << endl;
	return temp_long_key_plk;

}

std::vector<Visemes> VisemeAnalyzer::genRecordPad(string a_long_key)

{
	stringstream grp_stream;
	int sz = a_long_key.size();
	int vis_n = 0;
	char crnt_c = 'n';

	crnt_c = a_long_key[0];
	grp_stream.clear();
	grp_stream << crnt_c;
	grp_stream >> vis_n;
	std::vector<Visemes> temp_vv(1, Visemes(vis_n, 1));

	for (int i = 0; i < (a_long_key.size() - 1); i++)
	{
		if (a_long_key[i] == a_long_key[i + 1])
		{
			temp_vv[temp_vv.size() - 1].freq += 1;
		}
		else
		{
			crnt_c = a_long_key[i + 1];
			grp_stream.clear();
			grp_stream << crnt_c;
			grp_stream >> vis_n;
			temp_vv.push_back(Visemes(vis_n, 1));
		}

	}

	return temp_vv;

}


void VisemeAnalyzer::storeSelectedLongKeys ()
{
	cout << "Enter filename To store slctd long keys.(e.g. long_1.txt)" << endl;
	string tfn_SLK = "0";
	cin.ignore();
	cin >> tfn_SLK;
	tfn_SLK = RESOURCE_LOCATION_VA + tfn_SLK;

	ofstream SLK_ostream(tfn_SLK.c_str(), ios::out);
	if (SLK_ostream.is_open())
	{

		SLK_ostream << "_words\n\n";
		for (int k = 0; k < list_of_words.size(); k++)
		{
			SLK_ostream << list_of_words[k] << "\n";
		}
		SLK_ostream << "_wordsover\n\n";

		SLK_ostream << "_longkeys\n";

		for (int i = 0; i <choices_to_store.size(); i++)
		{
			
				SLK_ostream << long_keys[choices_to_store[i]] << "\n";
				
			

		}


		SLK_ostream << "_longkeysover\n";
		SLK_ostream.close();
		

	}
	else {
		cout << "Err in store longkeys";
	}


}

void VisemeAnalyzer::cleanDataMembers_1()
{
	long_keys.clear();
}


void VisemeAnalyzer::createTextChunk(const std::vector<string > & words_list, const std::vector<string > & related_keys,bool close_opstream)
{
	static int ctc_count = 0;
	stringstream cts_ss;
	string ctcfn = "0";
	static ofstream ctc_opstream;
	if (ctc_count == 0)
	{
		
		cout << "Enter File Name to store text chunks(e.g. text_chunk_1.txt)"<<endl;

		cin >> ctcfn;
		ctcfn = RESOURCE_LOCATION_VA + "TEXTCHUNKS/" + ctcfn;
		ctc_count++;

		ctc_opstream.open(ctcfn.c_str(), ios::out);
	}
	


	for (int k = 0; k < related_keys.size(); k++)
	{
		ctc_opstream << "_key\n";
		ctc_opstream << related_keys[k] << "\n\n";
		for (int m = 0; m < words_list.size(); m++)
		{
			ctc_opstream << words_list[m] << "\n";
		}

		ctc_opstream << "\n\n_over" << "\n\n";
	}

	if (close_opstream)
	{
		ctc_opstream.close();
		ctc_count = 0;
	}





}


void VisemeAnalyzer::createBigChunk()
{
	string cbc_fn_tmplt = "0";// template (commom part of the filname before index)
	string tempo_cbc_filename = "0";// temporary file names
	string cbc_dtf="0";//data transfer
	cout << "Enter File Name Template To Create Big Chunk (e.g. abc_ for abc_i.txt; i starting from 1)" << endl;
	//cin.ignore();
	cin >> cbc_fn_tmplt;
	stringstream cbc_ss;
	cout << "What's the number of files in the sequence?_ "<<endl;
	int cbcnum;
	cin.ignore();
	cin >> cbcnum;
	vector<string > cbc_temp_wl(1, "cbc_temp_wl");
	vector<string > cbc_temp_keys(1, "cbc_temp_keys");
	ifstream cbc_ifs;
	for (int k = 1; k <= cbcnum; k++)
	{
		cbc_ss.clear();
		cbc_ss << RESOURCE_LOCATION_VA << "TEXTCHUNKS/" << cbc_fn_tmplt << k << ".txt";
		cbc_ss >> tempo_cbc_filename;
		cbc_ifs.open(tempo_cbc_filename.c_str(), ios::in);
		
		cout <<"------k----\n"<< k << endl;
		cout << tempo_cbc_filename << endl;
		if (cbc_ifs.is_open())
		{
			while (cbc_ifs >> cbc_dtf)
			{
				if (cbc_dtf == "_words")
				{
					cbc_temp_wl.clear();
					while (true)
					{
						cbc_ifs >> cbc_dtf;
						if (cbc_dtf == "_wordsover")
						{
							break;
						}
						cbc_temp_wl.push_back(cbc_dtf);
						
					}
				}
				else if (cbc_dtf == "_longkeys")
				{
					cbc_temp_keys.clear();
					while (true)
					{
						cbc_ifs >> cbc_dtf;
						if (cbc_dtf == "_longkeysover")
						{
							break;
						}
						cbc_temp_keys.push_back(cbc_dtf);
					}
				}
				
			}//in if outer while ends here

			for (int d = 0; d < cbc_temp_keys.size(); d++)
			{
				if (k == cbcnum && d == (cbc_temp_keys.size() - 1))
				{
					createTextChunk(cbc_temp_wl, reduceByOPR(genRecordPad(cbc_temp_keys[d])), true);
				}
				else
					createTextChunk(cbc_temp_wl, reduceByOPR(genRecordPad(cbc_temp_keys[d])), false);
			}

		}
		else { cout << "IN createBigChunk: Unable to open file num" << k << endl; }

		

		cbc_ifs.close();
	}
}

bool VisemeAnalyzer::crudeAnalysis_OPR()
{
	removeAE_K(this->record_pad);
	//std::vector <string> temp_anls_str = reduceByOPR(reduceByRR(this->record_pad, groupVisemes_A(this->record_pad) ) );
	if( record_pad.size()>=3)
	{
		std::vector <string> temp_anls_str = reduceByOPR(this->record_pad);
	if (searchIn(temp_anls_str)) 
	{ 
		
		return true; 
	}

	cout << "By RR" << endl;
	record_pad=reduceByRR(record_pad, groupVisemes_A(record_pad));
	if (record_pad.size() >= 2)
	{
		temp_anls_str = reduceByOPR(record_pad);
		if (searchIn(temp_anls_str)) { return true; }
	}
	/*record_pad = reduceByIR(record_pad);
	if (record_pad.size() >= 2)
	{
		temp_anls_str = reduceByOPR(record_pad);
		if (searchIn(temp_anls_str)) { return true; }
	}*/

	cout << "By IR" << endl;
	while (record_pad.size() >= 2)
	{
		record_pad = reduceByIR(record_pad);
		if (record_pad.size() >= 2)
		{
			temp_anls_str = reduceByOPR(record_pad);
			if (searchIn(temp_anls_str)) { return true; }
		}
	}
	
	}

	
	
		cout << "Unknown Word\n";
		return false;
	
}

void VisemeAnalyzer::showVectViseme(const std::vector<Visemes> & vect_vsm)
{
	cout << "\n::::::::::::::::::\n";
	for (int i = 0; i < vect_vsm.size(); i++)
	{
		cout << "vis=" << vect_vsm[i].vis << ", fqc=" << vect_vsm[i].freq << endl;
	}
	cout << "\n-------------------\n";
}



void VisemeAnalyzer::createBigChunk_2()
{
	string cbc_fn_tmplt = "0";// template (commom part of the filname before index)
	string tempo_cbc_filename = "0";// temporary file names
	string cbc_dtf = "0";//data transfer
	cout << "Enter File Name Template To Create Big Chunk (e.g. abc_ for abc_i.txt; i starting from 1)" << endl;
	//cin.ignore();
	cin >> cbc_fn_tmplt;
	stringstream cbc_ss;
	cout << "What's the number of files in the sequence?_ " << endl;
	int cbcnum;
	cin.ignore();
	cin >> cbcnum;
	vector<string > cbc_temp_wl(1, "cbc_temp_wl");
	vector<string > cbc_temp_keys(1, "cbc_temp_keys");
	ifstream cbc_ifs;
	for (int k = 1; k <= cbcnum; k++)
	{
		cbc_ss.clear();
		cbc_ss << RESOURCE_LOCATION_VA << "TEXTCHUNKS/" << cbc_fn_tmplt << k << ".txt";
		cbc_ss >> tempo_cbc_filename;
		cbc_ifs.open(tempo_cbc_filename.c_str(), ios::in);

		cout << "------k----\n" << k << endl;
		cout << tempo_cbc_filename << endl;
		if (cbc_ifs.is_open())
		{
			while (cbc_ifs >> cbc_dtf)
			{
				if (cbc_dtf == "_words")
				{
					cbc_temp_wl.clear();
					while (true)
					{
						cbc_ifs >> cbc_dtf;
						if (cbc_dtf == "_wordsover")
						{
							break;
						}
						cbc_temp_wl.push_back(cbc_dtf);

					}
				}
				else if (cbc_dtf == "_longkeys")
				{
					cbc_temp_keys.clear();
					while (true)
					{
						cbc_ifs >> cbc_dtf;
						if (cbc_dtf == "_longkeysover")
						{
							break;
						}
						cbc_temp_keys.push_back(cbc_dtf);
					}
				}

			}//in if outer while ends here

			for (int d = 0; d < cbc_temp_keys.size(); d++)
			{
				std::vector<Visemes> vv_temp= genRecordPad(cbc_temp_keys[d]);
				//std::vector<int> parttn(0);
				removeAE_K(vv_temp);
				if (k == cbcnum && d == (cbc_temp_keys.size() - 1))
				{
					createTextChunk(cbc_temp_wl, reduceByOPR(vv_temp),false);
					while (vv_temp.size() >= 3)
					{
						//parttn = groupVisemes_A(vv_temp);
						vv_temp = reduceByRR(vv_temp, groupVisemes_A(vv_temp));
						if (vv_temp.size()>=2)
						{
							createTextChunk(cbc_temp_wl, reduceByOPR(vv_temp), false);
						}
						vv_temp = reduceByIR(vv_temp);
						if (vv_temp.size()>=2)
						{
							createTextChunk(cbc_temp_wl, reduceByOPR(vv_temp), true);
						}
					}
				}
				else
				{
					createTextChunk(cbc_temp_wl, reduceByOPR(vv_temp), false);
					while (vv_temp.size() >= 3)
					{
						//parttn = groupVisemes_A(vv_temp);
						vv_temp = reduceByRR(vv_temp, groupVisemes_A(vv_temp));
						if (vv_temp.size() >=2 )
						{
							createTextChunk(cbc_temp_wl, reduceByOPR(vv_temp), false);
						}

						vv_temp = reduceByIR(vv_temp);
						if (vv_temp.size()>=2)
						{
							createTextChunk(cbc_temp_wl, reduceByOPR(vv_temp), false);
						}

					}
				}
			}

		}
		else { cout << "IN createBigChunk: Unable to open file num" << k << endl; }



		cbc_ifs.close();
	}
}




void VisemeAnalyzer::createDictionary (string filename)
{
	try
	{
		ifstream input_stream(filename.c_str());
		//stringstream tss;
		string temp;
		string temp_key_s;
		//int temp_key;
		std::vector<string > temp_words(0);
		int noiid = 0;//number of items in the dictionary
		while (input_stream >> temp)
		{
			if (temp == "_key")
			{
				cout << temp << endl;
				noiid += 1;
				input_stream >> temp_key_s;
				//tss.clear();
				cout << temp_key_s << endl;

			}
			else if (temp == "_over")
			{
				//dictionary[temp_key] = temp_words;
				pair<mymap_s::iterator , bool> result = dictionary_s.insert(make_pair(temp_key_s , temp_words));
				if (!result.second)
				{
					dictionary_s[temp_key_s] = mergeTwoStringVectors(dictionary_s[temp_key_s], temp_words);
				}
				temp_words.clear();
			}
			else
			{

				temp_words.push_back(temp);
				cout << temp_words[temp_words.size() - 1] << endl;

			}


		}

		/*	for (mymap::iterator itr = dictionary.begin(); itr != dictionary.end(); ++itr)
		{
		cout << itr->first << endl;
		cout << " ---" << endl;
		for (int i = 0; i<itr->second.size(); i++)
		cout << itr->second[i] << endl;

		cout << "---------------------" << endl;
		}*/
		string loc_diction_save = RESOURCE_LOCATION_VA + "MAP_OF_DICTIONARY.txt";
		ofstream op_dict_save(loc_diction_save.c_str(), ios::out);
		cout << "-----------DICTIONARY_S-----------" << endl;
		for (mymap_s::iterator itr = dictionary_s.begin(); itr != dictionary_s.end(); ++itr)
		{
			cout << itr->first << endl;
			cout << " ---" << endl;
			for (int i = 0; i<itr->second.size(); i++)
				cout << itr->second[i] << endl;

			cout << "---------------------" << endl;
			//-------------------------------
			op_dict_save << itr->first << endl;
			op_dict_save << " ---" << endl;
			for (int i = 0; i<itr->second.size(); i++)
				op_dict_save << itr->second[i] << endl;

			op_dict_save << "---------------------" << endl;

			//-------------------------------
		}

		input_stream.close();
		op_dict_save.close();

	}
	catch (const std::exception&e)
	{
		cout << " ERR: " << e.what() << endl;
	}



}


std::vector<string > VisemeAnalyzer::mergeTwoStringVectors(const std::vector<string > & vect1,  std::vector<string >  vect2)
{
	for (int u = 0; u < vect1.size(); ++u)
	{
		for (int v = 0; v < vect2.size(); ++v)
		{
			if (vect1[u] == vect2[v])
			{
				vect2[v] = "0";
			}
		}
	}
	std::vector<string> vect_ret = vect1;
	for (int m = 0; m < vect2.size(); ++m)
	{
		if (vect2[m] != "0")
		{
			vect_ret.push_back(vect2[m]);
		}
	}

	return vect_ret;

}