// rapidjson Ʃ�丮��
// https://www.slideshare.net/PaulKim81/rapid-json-tutorial?ref=http://jacking.tistory.com/1431
// http://rapidjson.org/md_doc_tutorial.html Ʃ�丮��
// http://rapidjson.org/md_doc_pointer.html Json Pointer�� �߰� �ϴ� �κ�
// http://rapidjson.org/md_doc_stream.html  �� ��� �κ�

#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;

class CDataManager {
public:
	CDataManager() {}
	~CDataManager() {}
	
	Document loadData() {
		ifstream ifs("data/optiondata.json");
		IStreamWrapper isw(ifs);
		Document d;
		if (!ifs.is_open()) {
			Pointer("/open").Set(d, false);
			return d;
		}
		d.ParseStream(isw);
		Pointer("/open").Set(d, true);
		return d;
	}

	template<typename T>
	void saveData(char* pointer, T value) {
		Pointer(pointer).Set(document, value);
	}

	void outputData() {
		ofstream ofs("data/optiondata.json");
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		document.Accept(writer);
	}

private:
	Document document;
};
#endif

//#include "rapidjson\stringbuffer.h"
//#include "rapidjson\prettywriter.h"
//StringBuffer buffer;
//PrettyWriter<StringBuffer> writer(buffer);		// �� �ٲ� ����
//doc.Accept(writer);
//Writer<StringBuffer> w(buffer);					// �� �ٲ� ����
//doc.Accept(w);
//auto os = std::ofstream("data/test1", std::ofstream::out);
//os << buffer.GetString() << std::endl;
