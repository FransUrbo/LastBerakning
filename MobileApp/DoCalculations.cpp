/*
 * DoCalculations.cpp
 *
 * This is part of the MainScreen/screen.cpp class,
 * but in it's separate file to avoid clutter.
 *
 * $Id: DoCalculations.cpp,v 1.10 2010-04-24 09:19:01 turbo Exp $
 */

#include <MAFS/File.h>
#include <MAUtil/util.h>

#include <madmath.h>
#include <mastring.h>

#include "MAHeaders.h"
#include "screen.h"

// Main calculation method
void MainScreen::doCalculations() {
#if DEBUG >= 1
#if DEBUG >= 2
	lprintfln("------------------");
#endif
	lprintfln("Doing calculations...");
#endif

	/* ---------------------------------- */
	// Setup the general road weights (single axle weicles)
	int BK[6][2];
	BK[0][FRONT] = 10000;
	BK[0][BACK]  = 11500;
	BK[1][FRONT] = 10000;
	BK[1][BACK]  = 10000;
	BK[2][FRONT] =  8000;
	BK[2][BACK]  =  8000;

	/* ---------------------------------- */
	// Fetch input values
	fetchValues();

	/* ---------------------------------- */
	// We can't use the distance string, we need a Vector...
	if(truck_axle) {
#if DEBUG >= 1
		lprintfln("Truck axle dists: %s", truck_axle);
#endif
		weicle_dists[TRUCK]   = split("+", truck_axle);
	}
	if(trailer_axle) {
#if DEBUG >= 1
		lprintfln("Trailer axle dists: %s", trailer_axle);
#endif
		weicle_dists[TRAILER] = split("+", trailer_axle);
	}
#if DEBUG >= 1
	lprintfln("");
#endif

	/* ---------------------------------- */
	// Double check users claims about boggie/tripple axle...
	checkAxleTypeTruck();
	checkAxleTypeTrailer();

	/* ---------------------------------- */
	/* Calculate max axle load - GROSS_BK / MAX_AXLE */
	double total_dist, load[2];
	double GROSS_BK[2][3], MAX_AXLE[2][3], LOAD[3][3]; // 2=TRUCK/TRAILER, 3=BK1-3
	for(int bk = 0; bk < 3; bk++) {
		for(int type = 0; type < 2; type++) {
			if(weicle_dists[type].size()) {
				/* ---------------------------------- */
				// Get total axle distance
				total_dist = 0;
				for(int i = 0; i < weicle_dists[type].size(); i++)
					total_dist += weicle_dists[type][i];
#if DEBUG >= 1
				lprintfln("Total dist: %Lfmm", total_dist);
#endif

				/* ---------------------------------- */
				// Lookup BKx table value dep. on FIRST and LAST axle distance
				GROSS_BK[type][bk] = parseTable(TABLE_DATA[bk], total_dist, bk+1);
			} else
				GROSS_BK[type][bk] = 0;
#if DEBUG >= 1
			lprintfln("GROSS_BK[%d][BK%d] (%0.Lfm): %02.02Lf", type, bk+1, total_dist/1000, GROSS_BK[type][bk]);
			lprintfln("");
#endif

			/* ================================== */

			/* ---------------------------------- */
			// Get the lowest axle load allowed
			for(int location = 0; location < 2; location++) {
#if DEBUG >= 1
				lprintfln("Getting axle load on bk=%d for type=%d, at location=%d", bk+1, type, location);
#endif

				if(weicle_load[type][location]) {
					if(axle_type[type][location] == AXLE_SINGLE) {
#if DEBUG >= 1
						lprintfln("  single axle");
#endif
						if(weicle_load[type][location] <= BK[bk][location]) {
#if DEBUG >= 1
							lprintfln("    weicle_load[%d][%d] (%d) <= BK[%d][%d] (%d)",
									type, location, weicle_load[type][location], bk, location, BK[bk][location]);
#endif
							load[location] = weicle_load[type][location];
						} else {
#if DEBUG >= 1
							lprintfln("    weicle_load[%d][%d] (%d) >= BK[%d][%d] (%d)",
									type, location, weicle_load[type][location], bk, location, BK[bk][location]);
#endif
							load[location] = BK[bk][location];
						}
					} else if(axle_type[type][location] == AXLE_BOGGIE) {
#if DEBUG >= 1
						lprintfln("  boggie axle");
#endif
						int cnt = weicle_dists[type].size();
						if(location == 0) {
							// FRONT
							load[location] = parseTable(TABLE_DATA[3], weicle_dists[type][0], bk+1) * 1000;
						} else {
							// BACK
							load[location] = parseTable(TABLE_DATA[3], weicle_dists[type][cnt-1], bk+1) * 1000;
						}
					} else {
#if DEBUG >= 1
						lprintfln("  tripple axle");
#endif
						int cnt = weicle_dists[type].size();
						if(location == 0) {
							// FRONT
							load[location] = parseTable(TABLE_DATA[4], weicle_dists[type][0]+weicle_dists[type][1], bk+1) * 1000;
						} else {
							// BACK
							load[location] = parseTable(TABLE_DATA[4], weicle_dists[type][cnt-2]+weicle_dists[type][cnt-1], bk+1) * 1000;
						}
					}
				} else {
					// Just so that the train calc works below...
					load[location] = 0;
				}

#if DEBUG >= 1
				lprintfln("  load[%d]: %Lf", location, load[location]);
#endif
			}

			/* ---------------------------------- */
			// Calculate 'axle dist' for train (front of truck to back of trailer)
			MAX_AXLE[type][bk] = (load[FRONT] + load[BACK]) / 1000;
#if DEBUG >= 1
			lprintfln("MAX_AXLE[%d][BK%d]: %02.02Lf+%02.02Lf=%02.02Lf", type, bk+1, load[FRONT]/1000, load[BACK]/1000, MAX_AXLE[type][bk]);
			lprintfln("");
#endif
		}
	}

	/* ---------------------------------- */
	// Get number of axles
	int NR_AXLES[2];
	for(int type = 0; type < 2; type++) {
		NR_AXLES[type] = 0;

		for(int location = 0; location < 2; location++) {
#if DEBUG >= 1
			lprintfln("Axle type[%d][%d]: %d", type, location, axle_type[type][location]);
#endif
			switch(axle_type[type][location]) {
				case AXLE_SINGLE:
					// NOTE: Also matches if 'axle_type[type][location]' isn't set!
					NR_AXLES[type] += 1;
					break;

				case AXLE_BOGGIE:
					NR_AXLES[type] += 2;
					break;

				case AXLE_TRIPPLE:
					NR_AXLES[type] += 3;
					break;
			}
		}

#if DEBUG >= 1
		lprintfln("Number of axles (type=%d): %d", type, NR_AXLES[type]);
#endif
	}
#if DEBUG >= 1
	lprintfln("");
#endif

	/* ---------------------------------- */
	// Get Road load - LOAD
	for(int bk = 0; bk <= 2; bk++) {
		for(int type = 0; type < 2; type++) {
			/* ---------------------------------- */
			// Get the lowest denominator of GROSS_BK / MAX_AXLE
#if DEBUG >= 2
			lprintfln("GROSS_BK[%d][BK%d] (%02.02Lf) <= MAX_AXLE[%d][BK%d] (%02.02Lf)",
					type, bk+1, GROSS_BK[type][bk], type, bk+1, MAX_AXLE[type][bk]);
#endif
			if(GROSS_BK[type][bk] <= MAX_AXLE[type][bk])
				LOAD[type][bk] = GROSS_BK[type][bk];
			else
				LOAD[type][bk] = MAX_AXLE[type][bk];
#if DEBUG >= 1
			lprintfln("LOAD[%d][BK%d]: %02.02Lf", type, bk+1, LOAD[type][bk]);
#endif

			/* ---------------------------------- */
			// Check for common limitations
			if(LOAD[type][bk]) {
				if((NR_AXLES[type] == 2) && (LOAD[type][bk] > 18))
					// TWO axles
					LOAD[type][bk] = 18;

				else if((NR_AXLES[type] == 3) && !road_nice[type] && LOAD[type][bk] > 25)
					// THREE axles, NOT road nice
					LOAD[type][bk] = 25;

				else if((NR_AXLES[type] == 3) &&  road_nice[type] && LOAD[type][bk] > 26)
					// THREE axles, road nice
					LOAD[type][bk] = 26;

				else if((NR_AXLES[type] == 4) && !road_nice[type] && LOAD[type][bk] > 31)
					// THREE axles, NOT road nice
					LOAD[type][bk] = 31;

				else if((NR_AXLES[type] == 4) &&  road_nice[type] && LOAD[type][bk] > 32)
					// THREE axles, road nice
					LOAD[type][bk] = 32;
			} else {
				LOAD[type][bk] = 0;
			}

#if DEBUG >= 1
			lprintfln("=> LOAD[%d][BK%d]: %02.02Lf", type, bk+1, LOAD[type][bk]);
#endif
		}

		/* ---------------------------------- */
		// Calculate train load
		if(weicle_link[TRUCK] && weicle_link[TRAILER] && !weicle_load[TRAILER][FRONT])
			// Trailer - no front axle
			LOAD[TRAIN][bk] = parseTable(TABLE_DATA[bk], (weicle_link[TRUCK] + weicle_link[TRAILER]), bk+1);

		LOAD[TRAIN][bk] = LOAD[TRUCK][bk] + LOAD[TRAILER][bk];
#if DEBUG >= 1
		lprintfln("=> LOAD[%d][BK%d]: %02.02Lf", TRAIN, bk+1, LOAD[TRAIN][bk]);
#endif
	}
#if DEBUG >= 1
	lprintfln("");
#endif

	/* ---------------------------------- */
	// Final calculation(s)
	double weight[3];
	weight[TRUCK]   = (double)truck_weight/1000;
	weight[TRAILER] = (double)trailer_weight/1000;
#if DEBUG >= 1
	lprintfln("truck_weight=%02.02Lf (%02.02Lf), trailer_weight=%02.02Lf (%02.02Lf)",
			  truck_weight, weight[TRUCK], trailer_weight, weight[TRAILER]);
#endif
	for(int bk = 0; bk <= 2; bk++) {
		result_weight[TRUCK][bk]   = LOAD[TRUCK][bk];
		result_weight[TRAILER][bk] = LOAD[TRAILER][bk];
		result_weight[TRAIN][bk]   = LOAD[TRAIN][bk];

		result_load[TRUCK][bk]     = LOAD[TRUCK][bk] - weight[TRUCK];
		result_load[TRAILER][bk]   = LOAD[TRAILER][bk] - weight[TRAILER];
		result_load[TRAIN][bk]     = result_weight[TRAIN][bk] - (weight[TRUCK] + weight[TRAILER]);
#if DEBUG >= 1
		lprintfln("result_weight[TRAIN][BK%d]: %02.02Lf + %02.02Lf = %02.02Lf",
				bk+1, result_weight[TRUCK][bk], result_weight[TRAILER][bk], result_weight[TRAIN][bk]);
		lprintfln("result_load[TRAIN][BK%d]: %02.02Lf + %02.02Lf = %02.02Lf",
				bk+1, result_load[TRUCK][bk], result_load[TRAILER][bk], result_load[TRAIN][bk]);
#endif
	}

#if DEBUG >= 1
	lprintfln("------------------");
#endif
}

// Get values from EditBox'es
void MainScreen::fetchValues() {
	/* ---------------------------------- */
	// Get values we need - truck
#if DEBUG >= 1
	lprintfln("Fetching values - truck");
#endif
	truck_weight                = getIntValue(editBoxScreens[0]->editBox[0]);
	weicle_load[TRUCK][FRONT]   = getIntValue(editBoxScreens[0]->editBox[1]);
	axle_type[TRUCK][FRONT]     = (int)editBoxScreens[0]->group1->getSelectedButton();
	weicle_load[TRUCK][BACK]    = getIntValue(editBoxScreens[0]->editBox[2]);
	axle_type[TRUCK][BACK]      = (int)editBoxScreens[0]->group2->getSelectedButton();
	if(editBoxScreens[0]->checkBox[0]->isChecked())
		road_nice[TRUCK]        = TRUE;
	else
		road_nice[TRUCK]        = FALSE;
	truck_axle                  = getCharValue(editBoxScreens[0]->editBox[3]);
	weicle_link[TRUCK]          = getIntValue(editBoxScreens[0]->editBox[4]);

	/* ---------------------------------- */
	// Get values we need - trailer
#if DEBUG >= 1
	lprintfln("Fetching values - trailer");
#endif
	trailer_weight              = getIntValue(editBoxScreens[1]->editBox[0]);
	weicle_load[TRAILER][FRONT] = getIntValue(editBoxScreens[1]->editBox[1]);
	axle_type[TRAILER][FRONT]   = (int)editBoxScreens[1]->group1->getSelectedButton();
	weicle_load[TRAILER][BACK]  = getIntValue(editBoxScreens[1]->editBox[2]);
	axle_type[TRAILER][BACK]    = (int)editBoxScreens[1]->group2->getSelectedButton();
	if(editBoxScreens[1]->checkBox[0]->isChecked())
		road_nice[TRAILER]      = TRUE;
	else
		road_nice[TRAILER]      = FALSE;
	trailer_axle                = getCharValue(editBoxScreens[1]->editBox[3]);
	weicle_link[TRAILER]        = getIntValue(editBoxScreens[1]->editBox[4]);
}

// Check if front/back axle of truck is really tripple/boggie
void MainScreen::checkAxleTypeTruck()
{
	if(weicle_dists[TRUCK].size() && axle_type[TRUCK][FRONT] && axle_type[TRUCK][BACK]) {
		// Check if front axle type really is what user claims (check distances
		// between them). If not, override users distance claims (truck_axle)!
		checkAxleType(weicle_dists[TRUCK], axle_type[TRUCK][FRONT], true);
#if DEBUG >= 2
		for(int i=0; i < weicle_dists[TRUCK].size(); i++)
			lprintfln("    weicle_dists[TRUCK][%d] = '%Lf'", i, weicle_dists[TRUCK][i]);
#endif
#if DEBUG >= 1
		lprintfln("    TRUCK FRONT: '%d', BACK: '%d'", axle_type[TRUCK][FRONT], axle_type[TRUCK][BACK]);
#endif

		checkAxleType(weicle_dists[TRUCK], axle_type[TRUCK][BACK], false);
#if DEBUG >= 2
		for(int i=0; i < weicle_dists[TRUCK].size(); i++)
			lprintfln("    weicle_dists[TRUCK][%d] = '%Lf'", i, weicle_dists[TRUCK][i]);
#endif
#if DEBUG >= 1
		lprintfln("    TRUCK FRONT: '%d', BACK: '%d'", axle_type[TRUCK][FRONT], axle_type[TRUCK][BACK]);
#endif
	}
}

// Check if front/back axle of trailer is really tripple/boggie
void MainScreen::checkAxleTypeTrailer()
{
	if(weicle_dists[TRAILER].size() && axle_type[TRAILER][FRONT] && axle_type[TRAILER][BACK]) {
		// Check if front axle type really is what user claims (check distances
		// between them). If not, override users distance claims (trailer_axle)!
		checkAxleType(weicle_dists[TRAILER], axle_type[TRAILER][FRONT], true);
#if DEBUG >= 2
		for(int i=0; i < weicle_dists[TRAILER].size(); i++)
			lprintfln("    weicle_dists[TRAILER][%d] = '%d'", i, weicle_dists[TRAILER][i]);
#endif
#if DEBUG >= 1
		lprintfln("    TRAILER FRONT: '%d', BACK: '%d'", axle_type[TRAILER][FRONT], axle_type[TRAILER][BACK]);
#endif

		checkAxleType(weicle_dists[TRAILER], axle_type[TRAILER][BACK], false);
#if DEBUG >= 2
		for(int i=0; i < weicle_dists[TRAILER].size(); i++)
			lprintfln("    weicle_dists[TRAILER][%d] = '%d'", i, weicle_dists[TRAILER][i]);
#endif
#if DEBUG >= 1
		lprintfln("    TRAILER FRONT: '%d', BACK: '%d'", axle_type[TRAILER][FRONT], axle_type[TRAILER][BACK]);
#endif
	}
}

void MainScreen::checkAxleType(Vector<double> &axle_dists, int &axle_type, bool front)
{
	Vector<double> new_dist; // Temp variable
#if DEBUG >= 1
	lprintfln("checkAxleType(axle_dists, %d, %d)", axle_type, front);
#endif

	if(axle_dists.size() > 0) {
		switch(axle_type) {
			/* ---------------------------------- */
			case AXLE_TRIPPLE:
				// User claims to have a TRIPPLE axle at the front of the truck.
#if DEBUG >= 1
				lprintfln("  axle_type = tripple");
#endif

				// ERR (BOGGIE+TRIPPLE): 2020+3120+1360+1310 => 5140+1360+1310
				// ERR (TRIPPLE+BOGGIE): 1310+1360+3120+2020 => 1310+1360+5140

				if(front) {
					// TRIPPLE + SINGLE:  1520+1540+4185
					// TRIPPLE + BOGGIE:  1520+1540+4185+1360
					// TRIPPLE + TRIPPLE: 1520+1540+4185+1360+1310
#if DEBUG >= 1
					lprintfln("    checking front axles");
#endif
					if((axle_dists.size() >= 3) && ((axle_dists[0]+axle_dists[1]) > 5000)) {
						// NOT tripple axle!
						if(axle_dists[0] > 2000) {
							// NOT a boggie axle either => It's a single axle!
							axle_type = AXLE_SINGLE;

							new_dist.add(axle_dists[0]+axle_dists[1]+axle_dists[2]);
						} else {
							// It's a boggie axle
							axle_type = AXLE_BOGGIE;

							new_dist.add(axle_dists[0]);
							new_dist.add(axle_dists[1]+axle_dists[2]);
						}

						// Remove the first three elements.
						// Looks weird, but remove() reindexes...
						axle_dists.remove(0);
						axle_dists.remove(0);
						axle_dists.remove(0);

#if DEBUG >= 2
						for(int i = 0; i < axle_dists.size(); i++)
							lprintfln("      axle_dists[%d]: %Lf", i, axle_dists[i]);
#endif

						if(axle_dists[0])
							new_dist.add(axle_dists[0]);
						if(axle_dists[1])
							new_dist.add(axle_dists[1]);
						if(axle_dists[2])
							new_dist.add(axle_dists[2]);
					}
				} else {
					// SINGLE  + TRIPPLE:           4185+1520+1540
					// BOGGIE  + TRIPPLE:      1360+4185+1520+1540
					// TRIPPLE + TRIPPLE: 1310+1360+4185+1520+1540
#if DEBUG >= 1
					lprintfln("    checking back axles");
#endif
					int size = axle_dists.size();
					if((size >= 3) && ((axle_dists[size-1]+axle_dists[size]) > 5000)) {
						axle_type = AXLE_BOGGIE;

						for(int i = 0; i < size-2; i++)
							new_dist.add(axle_dists[i]);
						new_dist.add(axle_dists[size-2]+axle_dists[size-1]);
						axle_dists.remove(size-1);
					}
				}
				break;

			/* ---------------------------------- */
			case AXLE_BOGGIE:
				// User claims to have a BOGGIE axle at the front of the truck.
#if DEBUG >= 1
				lprintfln("  axle_type = boggie");
#endif

				if(front) {
					// BOGGIE + SINGLE:  1360+4185
					// BOGGIE + BOGGIE:  1360+4185+1360
					// BOGGIE + TRIPPLE: 1360+4185+1360+1310
#if DEBUG >= 1
					lprintfln("    checking front axles");
#endif
					if((axle_dists.size() >= 2) && (axle_dists[0] > 2000)) {
						axle_type = AXLE_SINGLE;

						new_dist.add(axle_dists[0]+axle_dists[1]);
						axle_dists.remove(1);
						if(axle_dists[1])
							new_dist.add(axle_dists[1]);
						if(axle_dists[2])
							new_dist.add(axle_dists[2]);
					}
				} else {
					// SINGLE  + BOGGIE:           4185+1360
					// BOGGIE  + BOGGIE:      1360+4185+1360
					// TRIPPLE + BOGGIE: 1310+1360+4185+1360
#if DEBUG >= 1
					lprintfln("    checking back axles");
#endif
					int size = axle_dists.size();
					if((size >= 2) && (axle_dists[size-1] > 2000)) {
						axle_type = AXLE_SINGLE;

						for(int i = 0; i < size-2; i++)
							new_dist.add(axle_dists[i]);
						new_dist.add(axle_dists[size-2]+axle_dists[size-1]);
						axle_dists.remove(size-1);
					}
				}
				break;
		}

		/* ---------------------------------- */
		if(new_dist.size())
			axle_dists = new_dist;
	}
}

// Load table files from 'builtin' FS
// http://www.mosync.com/documentation/manualpages/mafs-library-cc
// http://www.mosync.com/docs/r341/html/index.html
Vector<String> MainScreen::openTable(const char *name)
{
	char line[32];
	Vector<String> FILE_CONTENT;

	setCurrentFileSystem(RES_TABLE_FS, 0);

	MA_FILE *file = MA_fopen(name, "r");
	if(!file) {
		char *errstr;
		sprintf(errstr, "Could not open file '%s'!", name);
		maPanic(1, errstr);
	}

	while(MA_fgets(line, 32, file)) {
#if DEBUG >= 2
		lprintfln("Read line from '%s': '%s'", name, line);
#endif

		FILE_CONTENT.add(line);
	}
	if(!MA_feof(file)) {
		MA_fclose(file);
		freeCurrentFileSystem();

		maPanic(1, "Could not read string!");
	}

	MA_fclose(file);
	freeCurrentFileSystem();

	return(FILE_CONTENT);
}

// Parse table and find maximum load for 'dist' axle distance
double MainScreen::parseTable(Vector<String> data, double dist, int road)
{
	Vector<double> vals;

	// Convert distance to meters
	dist = dist / 1000;

#if DEBUG >= 1
	lprintfln("parseTable(data, %Lf, BK%d)", dist, road);
#endif

	for(int i = 0; i < data.size(); i++) {
#if DEBUG >= 2
		lprintfln("  data[%d]: %s", i, data[i].c_str());
#endif

    	// Split up the line in three data parts
    	// Ex: 11.00<11.25=44.00
    	vals = split("+<>=,", (char *)data[i].c_str());
#if DEBUG >= 2
    	for(int j = 0; j < vals.size(); j++)
    		lprintfln("  vals[%d]: '%Lf'", j, vals[j]);
#endif

    	if(data[i].find("+", 0) && (data[i].size() == 5)) {
    		// Special case for BK3! axle distance > 22 meter
    		// Ex:  22.00<+0.20=37.50+00.25
    		//
    		// [181] Looking for needles (+<>=) in heystack (22.00<+0.20=37.50+00.25):
    		// [181]   vals[0]: '22.000000'
    		// [181]   vals[1]: '0.000000'
    		// [181]   vals[2]: '0.200000'
    		// [181]   vals[3]: '37.500000'
    		// [181]   vals[4]: '0.250000'
    		double x = vals[0], y = vals[3];
    		for(; dist > x ; x += vals[2], y += vals[4]);
    		y -= vals[4];

#if DEBUG >= 1
    		lprintfln("  => axle distance > 22m (ret=%Lf)", y);
#endif
    		return(y);
    	} else {
    		if(	((dist > vals[0]) && (dist < vals[1])) ||
    			((dist > vals[0]) && (vals[1] == 0)) )
    		{
    			// Ex: 18.00>00.00=60.00
#if DEBUG >= 1
    			lprintfln("  find=%d, size=%d", data[i].find(",", 0), vals.size());
#endif
    			if(data[i].find(",", 0) && (vals.size() == 5)) {
    				// => Boggie- or Tripple axle
    				// Ex: 02.60<00.00=24.00,22.00,13.00
#if DEBUG >= 1
    				lprintfln("  => boggie/tripple axle (%s) (ret=%Lf)", data[i].c_str(), vals[1+road]); // if road=1 => 24.00
#endif
    				return(vals[1+road]);
    			} else {
    				// => Single axle
#if DEBUG >= 1
    				lprintfln("  => single axle (%s) (ret=%Lf)", data[i].c_str(), vals[2]);
#endif
    				return(vals[2]);
    			}
    		}
    	}
    }
}

// Split a string
//   Input:   String of needles to look for
//            String to split
//   Output:  Array of doubles
//
//   Example: 00.00<01.00=11.50
//            22.00<+0.20=37.50+00.25
//            1310+1360+3120+2020
Vector<double> MainScreen::split(const char *needles, char *heystack)
{
	Vector<double> ret_str;
	const char *val;
	String sub;

	if(strlen(heystack)) {
		String value = heystack;
		int pos = 0, j = 0;

#if DEBUG >= 2
		lprintfln("   needles (%s) in heystack (%s):", needles, value.c_str());
#endif
		for(int i = 0; i < value.size(); i++) {
			for(int j = 0; needles[j]; j++) {
				if(value[i] == needles[j]) {
					sub = value.substr(pos, i-pos);
					val = sub.c_str();

					ret_str.add(atof(val));

#if DEBUG >= 2
					lprintfln("    Found needle %c at %2d => %d+%d=%Lf (%s)",
							needles[j], i, pos, i-pos, ret_str[j], val);
#endif

					pos = i + 1;
					j++;
				}
			}
		}

		if(pos) {
			sub = value.substr(pos);
			val = sub.c_str();

			ret_str.add(atof(val));
#if DEBUG >= 2
			lprintfln("    Rest of string: '%Lf' (%s)", ret_str[j], val);
#endif
		}

#if DEBUG >= 2
		lprintfln("ret_str.size(): %d, value.find(): %d", ret_str.size(), value.find("+", 0));
#endif
		if((ret_str.size() == 0) && (value.find("+", 0) == -1)) {
			sub = value;
			val = sub.c_str();

			ret_str.add(atof(val));
#if DEBUG >= 1
			lprintfln("ret_str[0]: %Lf", ret_str[0]);
#endif
		}
	}

	return(ret_str);
}

int MainScreen::getIntValue(EditBox *e) {
	int val;
	char *str;
	String txt;

	txt = e->getText();
	str = (char *)txt.c_str();
	val = (int)atof(str);
#if DEBUG >= 1
	lprintfln("(1) val=%d, str=%s, strlen=%d, size=%d, length=%d)", val, str, strlen(str), txt.size(), txt.length());
#endif

	if(strlen(str) <= 0)
		val = 0;
#if DEBUG >= 1
	lprintfln("(2) val=%d", val);
#endif

	return(val);
}

char *MainScreen::getCharValue(EditBox *e) {
	char *str;
	int len = e->getText().size();

	if(len >= 1)
		str = (char *)e->getText().c_str();
	else
		str = NULL;

#if DEBUG >= 1
	lprintfln("String size: %d (%s)", len, str);
#endif
	return(str);
}
