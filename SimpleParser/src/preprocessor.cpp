#include "include/preprocessor.h"

namespace simple {
	bool preproc::proccomment() {
		string current;
		ifstream fin(path);
		ofstream fout(path + ".pre.nocomment");
		size_t tmp = 0;
		int row = 1;
		while (getline(fin, current)) {
			//handle comments
			if (current.find("/*") != current.npos) {
				if (current.find("/*") != current.npos) {
					state = COMMENT;
				}
				tmp = current.find("/*");
				fout << current.substr(0, tmp) << endl;
				continue;
			}
			if (current.find("*/") != current.npos) {
				state = NORMAL;
				tmp = current.find_last_of("*/");
				continue;
			}
			if (current.find("//") != current.npos) {
				tmp = current.find("//");
				fout << current.substr(0, tmp) << endl;
				continue;
			}
			if (state == COMMENT) {
				continue;
			}
			fout << current << endl;
			row++;
		}
		return true;
	}
	bool preproc::procdefine() {
		string current;
		ifstream fin(path+ ".pre.noinclude");
		ofstream fout(path + ".pre.nodefine");
		size_t tmp = 0;
		int row = 1;
		while (getline(fin, current)) {
			if (current.find("#") != current.npos) {
				if (current.find("#define") != current.npos) {
					istringstream cs(current);
					string sub;
					cs >> sub;
					define def;
					if (cs >> sub)
						def.def = sub;
					if (cs >> sub)
						def.substitute = sub;
					defines.append(def);
					continue;
				}
				else {
					size_t tmp = current.find("#");
					error err(row, tmp, "Preprocessor other than #define and #include not supported", current);
					err.print();
					return false;
				}
				continue;
			}
			for (int i = 0; i < defines.length; i++) {
				define def = defines[i];
				size_t defd = 0;
				while ((defd = current.find(def.def, defd)) != current.npos) {
					current.replace(defd, def.def.size(), def.substitute);
				}
			}
			fout << current << endl;
			row++;
		}
		return true;
	}
	int preproc::procinclude() { // 0: OK 1: error 2:further process required
		bool flag = false;
		string current;
		ifstream fin(path + ".pre.nocomment");
		ofstream fout(path + ".pre.noinclude");
		size_t tmp = 0;
		int row = 1;
		list<string> file;
		while (getline(fin, current)) {
			file.append(current);
		}
		ofstream fout2(path + ".pre.nocomment");

		for (int i = 0; i < file.length;i++) {
			current = file[i];
			if (current.find("#include") != current.npos) {
				flag = true;
				size_t l = current.find('<');
				size_t r = current.find('>');
				string incp = current.substr(l + 1, r - l - 1);
				ifstream inc(incp);
				if (!inc.is_open()) {
					size_t tmp = current.find("#");
					error err(row, tmp, "Cannot open include file", current);
					err.print();
					return 1;
				}
				else {
					string incc;
					while (getline(inc, incc)) {
						if (flag)
							fout2 << incc << endl;
						else
							fout << incc << endl;
					}
				}
				continue;
			}
			if (flag)
				fout2 << current << endl;
			else
				fout << current << endl;
			row++;
		}
		if (flag) return 2;
		return 0;
	}
	bool preproc::proc() {
		if (proccomment() == false) return false;
		int state;
		while (state = procinclude()) {
			if (state == 0) break;
			if (state == 2);
			if (state == 1) return false;
		}
		if (procdefine() == false) return false;
		return true;
	}
}