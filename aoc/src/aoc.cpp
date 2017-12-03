#include "stdafx.h"

struct advent_1 {
	void solve() {
		std::cout << part_1() << std::endl;
		std::cout << part_2() << std::endl;
	}

private:
	std::string input = "77736991856689225253142335214746294932318813454849177823468674346512426482777696993348135287531487622845155339235443718798255411492778415157351753377959586612882455464736285648473397681163729345143319577258292849619491486748832944425643737899293811819448271546283914592546989275992844383947572926628695617661344293284789225493932487897149244685921644561896799491668147588536732985476538413354195246785378443492137893161362862587297219368699689318441563683292683855151652394244688119527728613756153348584975372656877565662527436152551476175644428333449297581939357656843784849965764796365272113837436618857363585783813291999774718355479485961244782148994281845717611589612672436243788252212252489833952785291284935439662751339273847424621193587955284885915987692812313251556836958571335334281322495251889724281863765636441971178795365413267178792118544937392522893132283573129821178591214594778712292228515169348771198167462495988252456944269678515277886142827218825358561772588377998394984947946121983115158951297156321289231481348126998584455974277123213413359859659339792627742476688827577318285573236187838749444212666293172899385531383551142896847178342163129883523694183388123567744916752899386265368245342587281521723872555392212596227684414269667696229995976182762587281829533181925696289733325513618571116199419759821597197636415243789757789129824537812428338192536462468554399548893532588928486825398895911533744671691387494516395641555683144968644717265849634943691721391779987198764147667349266877149238695714118982841721323853294642175381514347345237721288281254828745122878268792661867994785585131534136646954347165597315643658739688567246339618795777125767432162928257331951255792438831957359141651634491912746875748363394329848227391812251812842263277229514125426682179711184717737714178235995431465217547759282779499842892993556918977773236196185348965713241211365895519697294982523166196268941976859987925578945185217127344619169353395993198368185217391883839449331638641744279836858188235296951745922667612379649453277174224722894599153367373494255388826855322712652812127873536473277";

	size_t advent_1_core(size_t offset) {
		size_t total = 0;
		for(size_t current = 0, next = offset, end = input.size(); current != end; ++current, ++next) {
			if(next == end) {
				next = 0;
			}
			if(input[current] == input[next]) {
				total += input[current] - '0';
			}
		}
		return total;
	}

	size_t part_1() {
		return advent_1_core(1);
	}

	size_t part_2() {
		return advent_1_core(input.size() / 2);
	}
};

struct advent_2 {
	using row = std::vector<std::size_t>;
	using spreadsheet = std::vector<row>;

	void solve() {
		//auto const ss = spreadsheet(
		//        input | ranges::view::split(is_char('\n'))
		//              | ranges::view::transform([](const std::string& line) -> row {
		//                return line | ranges::view::split(is_char('\t'))
		//                            | ranges::view::transform(ranges::convert_to<std::size_t>{});
		//                })
		//);

		std::vector<std::string> lines;
		boost::split(lines, input, [](char c) { return c == '\n'; });
		spreadsheet ss;
		for(const std::string& line : lines) {
			std::vector<std::string> cells;
			boost::split(cells, line, [](char c) { return c == '\t'; });
			row r;
			std::transform(begin(cells), end(cells), std::back_inserter(r), [](const std::string& s) { return std::stoull(s); });
			ss.push_back(r);
		}
		std::cout << part_1(ss) << std::endl;
		std::cout << part_2(ss) << std::endl;
	}

private:
	std::string input =
R"(1640	590	93	958	73	1263	1405	1363	737	712	1501	390	68	1554	959	79
4209	128	131	2379	2568	2784	2133	145	3618	1274	3875	158	1506	3455	1621	3799
206	1951	2502	2697	2997	74	76	78	1534	81	2775	2059	3026	77	2600	3067
373	1661	94	102	2219	1967	1856	417	1594	75	100	2251	2200	1825	1291	1021
57	72	51	1101	1303	60	1227	421	970	1058	138	333	1320	1302	402	1210
4833	5427	179	3934	4533	5124	4832	2088	94	200	199	1114	4151	1795	208	3036
759	876	110	79	1656	1691	185	544	616	312	757	1712	92	97	1513	1683
1250	1186	284	107	1190	1233	573	1181	1041	655	132	547	395	146	119	515
505	1726	79	180	86	1941	1597	1785	1608	1692	968	1177	94	184	91	31
1366	2053	1820	1570	70	506	53	415	717	1263	82	366	74	1255	2020	1985
2365	5585	2285	4424	5560	3188	3764	187	88	223	1544	5023	4013	5236	214	196
1487	1305	1359	1615	6579	2623	4591	150	5030	188	146	4458	5724	5828	1960	221
3114	688	3110	334	1921	153	4083	131	2234	3556	3573	3764	127	919	3293	104
1008	78	1196	607	135	1409	296	475	915	157	1419	1304	153	423	163	704
235	4935	4249	3316	1202	221	1835	380	249	1108	1922	5607	4255	238	211	3973
1738	207	179	137	226	907	1468	1341	1582	1430	851	213	393	1727	1389	632)";

	std::size_t part_1(const spreadsheet& ss) {
		std::size_t running_checksum = 0;

		for(const row& r : ss) {
			auto mm = std::minmax_element(begin(r), end(r));
			running_checksum += *mm.second - *mm.first;
		}
		return running_checksum;
	}

	std::size_t part_2(const spreadsheet& ss) {
		std::size_t running_sum = 0;
		for(const row& r : ss) {
			for(size_t i = 0, end = r.size(); i != end; ++i) {
				for(size_t j = 0; j != end; ++j) {
					if(i == j) {
						continue;
					}
					if(r[i] % r[j] == 0) {
						running_sum += r[i] / r[j];
					}
				}
			}
		}
		return running_sum;
	}
};

struct advent_3 {
	void solve() {
		std::cout << part_1() << std::endl;
		std::cout << part_2() << std::endl;
	}

private:
	std::ptrdiff_t input = 312'051;

	template<typename F>
	void spiral_iterate(F&& fn) {
		std::ptrdiff_t dx = 1;
		std::ptrdiff_t dy = 0;
		for(std::ptrdiff_t size = 1; ; ++size) {
			for(std::ptrdiff_t y = dy; y < dx; ++y) {
				if(!fn(dx, y)) {
					return;
				}
			}
			dy = dx;
			for(std::ptrdiff_t x = dx; x > -dx; --x) {
				if(!fn(x, dy)) {
					return;
				}
			}
			dx = -dx;
			for(std::ptrdiff_t y = dy; y > -dy; --y) {
				if(!fn(dx, y)) {
					return;
				}
			}
			dy = -dy;
			for(std::ptrdiff_t x = dx; x < -dx + 1; ++x) {
				if(!fn(x, dy)) {
					return;
				}
			}
			dx = -dx + 1;
		}
	}

	std::ptrdiff_t part_1() {
		std::ptrdiff_t current = 1;
		std::ptrdiff_t distance = 0;
		spiral_iterate([&](std::ptrdiff_t x, std::ptrdiff_t y) {
			if(++current == input) {
				distance = std::abs(x) + std::abs(y);
				return false;
			}
			return true;
		});
		return distance;
	}

	using coordinate = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

	struct coordinate_hash {
		std::size_t operator()(const coordinate& c) const noexcept {
			return std::hash<std::ptrdiff_t>{}(c.first) | std::hash<std::ptrdiff_t>{}(c.second);
		}
	};

	using visited_map = std::unordered_map<coordinate, std::ptrdiff_t, coordinate_hash>;

	std::ptrdiff_t sum_neighbours(const visited_map& visited, std::ptrdiff_t x, std::ptrdiff_t y) {
		auto end = visited.end();
		decltype(end) it;
		std::ptrdiff_t sum = ((it = visited.find({ x - 1, y - 1 })) != end ? it->second : 0)
		                   + ((it = visited.find({ x - 1, y     })) != end ? it->second : 0)
		                   + ((it = visited.find({ x - 1, y + 1 })) != end ? it->second : 0)
		                   + ((it = visited.find({ x    , y - 1 })) != end ? it->second : 0)
		                   + ((it = visited.find({ x    , y + 1 })) != end ? it->second : 0)
		                   + ((it = visited.find({ x + 1, y - 1 })) != end ? it->second : 0)
		                   + ((it = visited.find({ x + 1, y     })) != end ? it->second : 0)
		                   + ((it = visited.find({ x + 1, y + 1 })) != end ? it->second : 0)
		;
		return sum;
	}

	std::ptrdiff_t part_2() {
		visited_map visited;
		visited[coordinate{ 0, 0 }] = 1;
		std::ptrdiff_t next_highest = 0;
		spiral_iterate([&](std::ptrdiff_t x, std::ptrdiff_t y) {
			std::ptrdiff_t value = sum_neighbours(visited, x, y);
			if(value > input) {
				next_highest = value;
				return false;
			}
			visited[coordinate{ x, y }] = value;
			return true;
		});
		return next_highest;
	}
};

int main() {
	advent_1 a1;
	a1.solve();

	advent_2 a2;
	a2.solve();

	advent_3 a3;
	a3.solve();

	return 0;
}
