<?php
// Values: 0-4
$DEBUG = 3;

// Calculate max load and gross load on truck and/or trailer
// $Id: index.php,v 1.16 2010-02-22 22:34:40 turbo Exp $

$VERSION = "$Revision: 1.16 $";

// {{{ Defines
// For Single axles only !!
$BK["1"]["FRONT"] = 10;
$BK["1"]["BACK"]  = 11.5;
$BK["2"]["FRONT"] = 10;
$BK["2"]["BACK"]  = 10;
$BK["3"]["FRONT"] =  8;
$BK["3"]["BACK"]  =  8;
// }}}

// {{{ printr(string)
function printr($string) {
  echo "<pre>\n";
  
  if(is_array($string))
	print_r($string);
  else
	echo "$string<br>";
  
  echo "</pre><p>\n";
}
// }}}

// {{{ Parse gross load table
// NOTE: $bk is only used if we're checking a boggie/tripple
//       axle veicle...
function parse_table($data, $distance, $bk = 0) {
  global $DEBUG;

  // Tables is in meters, but form is in milimeter...
  $d = $distance / 1000;
  if($DEBUG >= 2)
	echo "&nbsp;&nbsp;Distance: '$d'm<br>";

  for($i = 0; $data[$i] and !$RET; $i++) {
	// Split up the line in three data parts
	// Ex:	11.00<11.25=44.00
	$vals = preg_split("/[\<\>\=]/", $data[$i]);
	if($DEBUG >= 3) {
	  echo "&nbsp;&nbsp;&nbsp;&nbsp;";
	  print_r($vals);
	  echo "<br>";
	}

	// Remove any preceding zeros
	for($j = 0; $j <= 2; $j++) {
	  if($vals[$j] == '00.00')
		$vals[$j] = 0;
	  else
		$vals[$j] = preg_replace("/^0/", "", $vals[$j]);
	}

	$chars = preg_split("//", $data[$i]);
	if($chars[6] == '<') {
	  if($chars[7] == '+') {
		// Special case for BK3! axle distance > 22 meter
		// Ex:	22.00<+0.20=37.50+00.25
		$res = preg_split("/\+/", $vals[2]);

		for($j = $vals[0], $k = $res[0]; $d > $j; $j += $vals[1], $k += $res[1]);

		$j -= $vals[1]; $k -= $res[1];
		$RET = $k;
	  } else {
		if( (($d >= $vals[0]) and ($d < $vals[1])) or
			(($d >= $vals[0]) and ($vals[1] == '0')) ) {
		  if(preg_match("/,/", $vals[2])) {
			// => Boggie- or Trippleaxle
			if($DEBUG >= 2)
			  echo "&nbsp;&nbsp;Matching boggie/tripple axle (".$vals[2].")<br>";

			$real_vals = preg_split("/,/", $vals[2]);
			$RET = $real_vals[$bk-1];
		  } else {
			if($DEBUG >= 2)
			  echo "&nbsp;&nbsp;Matching single axle (".$vals[2].")<br>";

			$RET = $vals[2];
		  }
		}
	  }
	} elseif($chars[6] == '>') {
	  // BK1 road with axle distance >= 18.00 meters
	  // Ex:	18.00>00.00=60.00
	  if( $d >= $vals[0] )
		$RET = $vals[2];
	}
  }

  return($RET);
}
// }}}

if(!$_REQUEST["action"]) {
  // {{{ Retreive and parse cookie(s)
  foreach(array("truck", "trailer") as $type) {
	$str = "LastBerakning:$type";
	if($_COOKIE[$str]) {
	  $cookie = preg_split("/:/", $_COOKIE[$str]);
	  for($i=0; $cookie[$i]; $i++) {
		$val = preg_split("/=/", $cookie[$i]);
		$_REQUEST[$val[0]] = $val[1];
	  }
	  unset($_REQUEST[$str]);

	  if($DEBUG >= 3)
		printr($_REQUEST);
	}
  }
  // }}}

  // {{{ Input form
?>
<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1">
    <link rel="stylesheet" type="text/css" href="tooltip.css">
  </head>
  <body>
    <table>
      <form method="post" name="berakning":
        <tr>
          <td colspan="4"><h1>Bil:</h1></td>
          <td align="right"><input type="checkbox" name="truck_save">Spara</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td colspan="4"><h1>Sl&auml;p:</h1></td>
          <td align="right"><input type="checkbox" name="trailer_save">Spara</td>
        </tr>

        <tr>
          <td>&nbsp;</td>
          <td>Tj&auml;nstevikt</td>
          <td>
            <a class=info href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset
              </span>
            </a>
          </td>
          <td><input type="text" name="truck_weight" tabindex="1" value="<?php echo $_REQUEST["truck_weight"]; ?>"></td>
          <td>Ton</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>&nbsp;</td>
          <td>Tj&auml;nstevikt</td>
          <td>
            <a class=info href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset
              </span>
            </a>
          </td>
          <td><input type="text" name="trailer_weight" tabindex="11" value="<?php echo $_REQUEST["trailer_weight"]; ?>"></td>
          <td>Ton</td>
        </tr>

        <tr>
          <td rowspan="2">Framaxel</td>
          <td>Max belastning</td>
          <td>
            <a class="info" href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset
              </span>
            </a>
          </td>
          <td><input type="text" name="truck_load_front" tabindex="2" value="<?php echo $_REQUEST["truck_load_front"]; ?>"></td>
          <td>Ton</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td rowspan="2">Framaxel</td>
          <td>Max belastning</td>
          <td>
            <a class="info" href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset.<br>
                Om trailer, skriv <b>0</b> som vikt.
              </span>
            </a>
          </td>
          <td><input name="trailer_load_front" tabindex="12" value="<?php echo $_REQUEST["trailer_load_front"]; ?>" type="text"></td>
          <td>Ton</td>
        </tr>
        <tr>
          <td>Axeltyp</td>
          <td>
            <a class=info href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset
              </span>
            </a>
          </td>
          <td>
            <select name="truck_axles_front">
              <option name="truck_axles_front_single" value="single"<?php if($_REQUEST["truck_axles_front"] == 'single') { echo " SELECTED"; } ?>>Enkelaxel</option>
              <option name="truck_axles_front_boggie" value="boggie"<?php if($_REQUEST["truck_axles_front"] == 'boggie') { echo " SELECTED"; } ?>>Boggie</option>
              <option name="truck_axles_front_tripple" value="tripple"<?php if($_REQUEST["truck_axles_front"] == 'tripple') { echo " SELECTED"; } ?>>Trippleaxel</option>
            </select>
          </td>
          <td>&nbsp;</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>Axeltyp</td>
          <td>
            <a class=info href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset<br>
                Om trailer, skriv <b>0</b> som vikt
              </span>
            </a>
          </td>
          <td>
            <select name="trailer_axles_front">
              <option name="trailer_axles_front_single" value="single"<?php if($_REQUEST["trailer_axles_front"] == 'single') { echo " SELECTED"; } ?>>Enkelaxel</option>
              <option name="trailer_axles_front_boggie" value="boggie"<?php if($_REQUEST["trailer_axles_front"] == 'boggie') { echo " SELECTED"; } ?>>Boggie</option>
              <option name="trailer_axles_front_tripple" value="tripple"<?php if($_REQUEST["trailer_axles_front"] == 'tripple') { echo " SELECTED"; } ?>>Trippleaxel</option>
            </select>
          </td>
          <td>&nbsp;</td>
        </tr>

        <tr>
          <td rowspan="2">Bakaxel</td>
          <td>Max belastning</td>
          <td>
            <a class=info href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset
              </span>
            </a>
          </td>
          <td><input type="text" name="truck_load_back" tabindex="3" value="<?php echo $_REQUEST["truck_load_back"]; ?>"></td>
          <td>Ton</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td rowspan="2">Bakaxel</td>
          <td>Max belastning</td>
          <td>
            <a class=info href="#">?
              <span>
                Vikt i ton fr&aring;n registrerings beviset
              </span>
            </a>
          </td>
          <td><input type="text" name="trailer_load_back" tabindex="13" value="<?php echo $_REQUEST["trailer_load_back"]; ?>"></td>
          <td>Ton</td>
        </tr>

        <tr>
          <td>Axeltyp</td>
          <td>
            <a class=info href="#">?
              <span>
                Bakaxel typ (En, tv&aring; eller tre bakaxlar)
              </span>
            </a>
          </td>
          <td>
            <select name="truck_axles_back">
              <option name="truck_axles_back_single" value="single"<?php if($_REQUEST["truck_axles_back"] == 'single') { echo " SELECTED"; } ?>>Enkelaxel</option>
              <option name="truck_axles_back_boggie" value="boggie"<?php if($_REQUEST["truck_axles_back"] == 'boggie') { echo " SELECTED"; } ?>>Boggie</option>
              <option name="truck_axles_back_tripple" value="tripple"<?php if($_REQUEST["truck_axles_back"] == 'tripple') { echo " SELECTED"; } ?>>Trippleaxel</option>
            </select>
          </td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>Axeltyp</td>
          <td>
            <a class=info href="#">?
              <span>
                Bakaxel typ (En, tv&aring; eller tre bakaxlar)
              </span>
            </a>
          </td>
          <td>
            <select name="trailer_axles_back">
              <option name="trailer_axles_back_single" value="single"<?php if($_REQUEST["truck_axles_back"] == 'single') { echo " SELECTED"; } ?>>Enkelaxel</option>
              <option name="trailer_axles_back_boggie" value="boggie"<?php if($_REQUEST["truck_axles_back"] == 'boggie') { echo " SELECTED"; } ?>>Boggie</option>
              <option name="trailer_axles_back_tripple" value="tripple"<?php if($_REQUEST["truck_axles_back"] == 'tripple') { echo " SELECTED"; } ?>>Trippleaxel</option>
            </select>
          </td>
        </tr>

        <tr>
          <td>&nbsp;</td>
          <td>V&auml;gv&auml;nlig fj&auml;dring</td>
          <td>
            <a class=info href="#">?
              <span>
                Tvillingmontage och luftfj&auml;dring
              </span>
            </a>
          </td>
          <td><input type="checkbox" name="truck_road_nice" tabindex="7"<?php if($_REQUEST["truck_road_nice"]) { echo " CHECKED"; } ?>></td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
        </tr>

        <tr>
          <td>&nbsp;</td>
          <td>Axelavst&aring;nd</td>
          <td>
            <a class=info href="#">?
              <span>
                * <b>Enkelaxel</b><br>
                &nbsp;&nbsp;&nbsp;Avst&aring;ndet mellan framaxel och bakaxel<br>
                &nbsp;&nbsp;&nbsp;Exempel (skriv in exakt s&aring; h&auml;r!): <b>5900</b><p>

                * <b>Boggieaxel</b><br>
                &nbsp;&nbsp;&nbsp;Avst&aring;ndet mellan framaxel och f&ouml;rsta bakaxeln <b>+</b><br>
                &nbsp;&nbsp;&nbsp;avst&aring;ndet mellan f&ouml;rsta och sista bakaxeln<br>
                &nbsp;&nbsp;&nbsp;Exempel (skriv in exakt s&aring; h&auml;r!): <b>4900+1350</b><p>

                * <b>Trippelaxel</b><br>
                &nbsp;&nbsp;&nbsp;Avst&aring;ndet mellan framaxel och f&ouml;rsta bakaxeln<br>
                &nbsp;&nbsp;&nbsp;<b>+</b> avst&aring;ndet mellan f&ouml;rsta och andra bakaxeln <b>+</b><br>
                &nbsp;&nbsp;&nbsp;avst&aring;ndet mellan andra och sista (tredje) bakaxeln<br>
                &nbsp;&nbsp;&nbsp;Exempel (skriv in exakt s&aring; h&auml;r!): <b>3750+1360+1310</b><p>

                Programmet r&auml;knar automatiskt ut dom avst&aring;nd det beh&ouml;ver fr&aring;n dessa
              </span>
            </a>
          </td>
          <td><input type="text" name="truck_axle" tabindex="4" value="<?php echo $_REQUEST["truck_axle"]; ?>"></td>
          <td>mm</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>&nbsp;</td>
          <td>Axelavst&aring;nd</td>
          <td>
            <a class=info href="#">?
              <span>
                * <b>Enkelaxel</b><br>
                &nbsp;&nbsp;&nbsp;Avst&aring;ndet mellan framaxel och bakaxel<br>
                &nbsp;&nbsp;&nbsp;Exempel (skriv in exakt s&aring; h&auml;r!): <b>1820</b><p>

                * <b>Boggieaxel</b><br>
                &nbsp;&nbsp;&nbsp;Avst&aring;ndet mellan framaxel och f&ouml;rsta bakaxeln <b>+</b><br>
                &nbsp;&nbsp;&nbsp;avst&aring;ndet mellan f&ouml;rsta och sista bakaxeln<br>
                &nbsp;&nbsp;&nbsp;Exempel (skriv in exakt s&aring; h&auml;r!): <b>6400+2020</b><p>

                * <b>Trippelaxel</b><br>
                &nbsp;&nbsp;&nbsp;Avst&aring;ndet mellan framaxel och f&ouml;rsta bakaxeln<br>
                &nbsp;&nbsp;&nbsp;<b>+</b> avst&aring;ndet mellan f&ouml;rsta och andra bakaxeln <b>+</b><br>
                &nbsp;&nbsp;&nbsp;avst&aring;ndet mellan andra och sista (tredje) bakaxeln<br>
                &nbsp;&nbsp;&nbsp;Exempel (skriv in exakt s&aring; h&auml;r!): <b>3750+1360+1310</b><p>

                Programmet r&auml;knar automatiskt ut dom avst&aring;nd det beh&ouml;ver fr&aring;n dessa
              </span>
            </a>
          </td>
          <td><input type="text" name="trailer_axle" tabindex="14" value="<?php echo $_REQUEST["trailer_axle"]; ?>"></td>
          <td>mm</td>
        </tr>

        <tr>
          <td>&nbsp;</td>
          <td>Kopplingsavst&aring;nd</td>
          <td>
            <a class=info href="#">?
              <span>
                Avst&aring;nd fr&aring;n framaxel till kopplingspunkt. Endast n&ouml;dv&auml;ndig om fordonst&aring;g ber&auml;knas
              </span>
            </a>
          </td>
          <td><input type="text" name="truck_link" tabindex="5" value="<?php echo $_REQUEST["truck_link"]; ?>"></td>
          <td>mm</td>
          <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
          <td>&nbsp;</td>
          <td>Kopplingsavst&aring;nd</td>
          <td>
            <a class=info href="#">?
              <span>
                Avst&aring;nd fr&aring;n framaxel till kopplingspunkt. Endast n&ouml;dv&auml;ndig om fordonst&aring;g ber&auml;knas
              </span>
            </a>
          </td>
          <td><input type="text" name="trailer_link" tabindex="15" value="<?php echo $_REQUEST["trailer_link"]; ?>"></td>
          <td>mm</td>
        </tr>

        <!-- ---------------------------------- -->

       <tr>
          <td>&nbsp;</td>
          <td>
            <input type="submit" name="action" value="submit">
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <input type=reset name=action value=reset>
          </td>
        </tr>
      </form>
    </table>
  </body>
</html>
<?php
// }}}
} else {
  // {{{ If DEBUG - print keys / values from form
  if($DEBUG) {
	foreach($_REQUEST as $key => $val) {
	  echo "$key => $val<br>";
	}
	echo "--------------<p>";
  }
  // }}}

  // {{{ Save truck/trailer
  foreach(array("truck", "trailer") as $type) {
	$str = $type."_save";
	if($_REQUEST[$str]) {
	  $cookie = "";

	  foreach($_REQUEST as $key => $val) {
		if(preg_match("/^$type/", $key) and !preg_match("/_save$/", $key))
		  $cookie .= "$key=$val:";
	  }

	  if(!$DEBUG)
		// Can't set cookie if debuging - output already sent!
		setcookie("LastBerakning:$type", $cookie, 0, $_SERVER["REQUEST_URI"]);
	}
  }
  // }}}

  // {{{ 1. Load BKx table file(s)
  foreach(array("bk1", "bk2", "bk3", "boggie", "tripple") as $bk) {
	$nr = preg_replace("/^bk([1-3])/", "$1", $bk);
	$name = "table-$bk".".txt";
	$fh = fopen($name, 'r') or die("Kan inte öppna bruttoviktstabell filerna!<br>");

	$j = 0;
	while($line = fgets($fh)) {
	  $TABLE_DATA[$nr][$j] = trim($line);

	  $j++;
	}

	fclose($fh);
  }

  if($DEBUG >= 4) {
	echo "<b>BK table data (\$TABLE_DATA)</b>:";
	printr($TABLE_DATA);
  }
  // }}}


  // {{{ 2. $GROSS_BK
  $TRAILER = 0;

  // Lookup BKx table value dep. on axle distance
  if($DEBUG)
	echo "<b>Gross BK values (\$GROSS_BK)</b>:<br>";

  for($bk = 1; $bk <= 3; $bk++) {
	foreach(array("truck", "trailer") as $type) {
	  $str = $type."_axles_back";

	  // {{{ Figure out if it's really a boogie/tripple axle
	  foreach(array("front", "back") as $location) {
		$key1 = $type.'_axles_'.$location;
		if($_REQUEST[$key1]) {
		  $key2 = $type."_axle";

		  if($_REQUEST[$key1] == 'tripple') {
			// {{{ If the distance between the first and last axle in the tripple
			// is > 5 meters, it's NOT a tripple, but a boggie!
			// EX: 3750+2990+2020
			if(preg_match("/\+.*\+/", $_REQUEST[$key2])) {
			  $dist = preg_split("/\+/", $_REQUEST[$key2]);

			  if(($dist[1]+$dist[2]) > 5000) {
				$_REQUEST[$key1] = 'boggie';
				$_REQUEST[$key2] = $dist[0]+$dist[1]."+".$dist[2];
			  }
			}
			// }}}
		  } elseif($_REQUEST[$key1] == 'boggie') {
			// {{{ If the distance between the first and last axle in the boggie
			// is > 2 meters, it's NOT a boggie, but a single!
			// EX: 4900+2020
			if(preg_match("/\+/", $_REQUEST[$key2])) {
			  $dist = preg_split("/\+/", $_REQUEST[$key2]);

			  if($dist[1] > 2000) {
				$_REQUEST[$key1] = 'single';
				$_REQUEST[$key2] = $dist[0] + $dist[1];
			  }
			}
			// }}}
		  }
		}
	  }
	  // }}}
	  
	  if(($type == 'trailer') and ($_REQUEST["trailer_load_front"] == '0')) {
		// {{{ Trailer - no front axles!
		$TRAILER = 1;
		
		// Force trailer_axles_front to single
		$_REQUEST["trailer_axles_front"] = 'single';
		// }}}
	  } else {
		// {{{ Trailer - with front axle OR Truck!
		if($_REQUEST[$str] == "single") {
		  // {{{ => SINGLE
		  if($DEBUG >= 2)
			echo "Single axle (bk$bk / $type)<br>";
		  
		  $str1 = $type."_axle";
		  if($_REQUEST[$str1])
			$GROSS_BK[$bk][$type] = parse_table($TABLE_DATA[$bk], $_REQUEST[$str1]);
		  // }}}
		} elseif(($_REQUEST[$str] == "boggie") or ($_REQUEST[$str] == "tripple")) {
		  // {{{ => BOGGIE/TRIPPLE
		  if($DEBUG >= 2)
			echo $_REQUEST[$str]." axle (bk$bk / $type)<br>";
		  
		  $str1 = $type."_axle";
		  
		  if($_REQUEST[$str1]) {
			$dist = $_REQUEST[$str1];
			if($dist)
			  eval("\$dist = ".htmlspecialchars($dist).";");
			
			$GROSS_BK[$bk][$type] = parse_table($TABLE_DATA[$bk], $dist, $bk);
		  } elseif($DEBUG >= 2) {
			  echo "&nbsp;&nbsp;n/a<br>";
			  $GROSS_BK[$bk][$type] = 0;
		  }
		  // }}}
		}
		  
		if($DEBUG >= 2)
		  echo "<br>";
		// }}}
	  }
	}

	if($DEBUG >= 2)
	  echo "--------------<p>";
  }	

  if($DEBUG)
	printr($GROSS_BK);
  // }}}

  // {{{ 3. $MAX_AXLE
  // Calculate max axle load
  if($DEBUG)
	echo "<b>Max axle load (\$MAX_AXLE)</b>:<br>";

  for($bk = 1; $bk <= 3; $bk++) {
	foreach(array("truck", "trailer") as $type) {
	  // {{{ Get the lowest axle load allowed
	  foreach(array("front", "back") as $location) {
		$str = $type.'_load_'.$location;
		$load = 'load_'.$location;

		if($_REQUEST[$str]) {
		  $key = $type."_axles_".$location;
		  if($_REQUEST[$key] == 'single') {
			// {{{ => Single axle
			if($DEBUG >= 3)
			  echo "Single axle (bk$bk / $type): _REQUEST[$str] (".$_REQUEST[$str].") <= BK[$bk][".strtoupper($location)."] (".$BK[$bk][strtoupper($location)].")<br>";
			
			($_REQUEST[$str] <= $BK[$bk][strtoupper($location)])  ? $$load  = $_REQUEST[$str]  : $$load  = $BK[$bk][strtoupper($location)];
			// }}}
		  } else {
			// {{{ => Boggie/Tripple axle
			$key = $type."_axles_".$location;
			if($_REQUEST[$key] == "boggie")
			  $axle = "boggie";
			elseif($_REQUEST[$key] == "tripple")
			  $axle = "tripple";
			
			if($DEBUG >= 2)
			  echo "$axle axle (bk$bk / $type / $location)<br>";
			
			// {{{ Extract axle distances from form
			$str2 = $type."_axle";
			if(preg_match("/\+.*\+.*\+/", $_REQUEST[$str2])) {
			  // {{{ Front boggie, back tripple
			  // EX: 2020+3120+1360+1310
			  if($axle == 'boggie') {
				// => First one...
				$dist = preg_split("/\+/", $_REQUEST[$str2]);
				$dist = $dist[0];
			  } elseif($axle == 'tripple') {
				// => Last two...
				$dist = preg_split("/\+/", $_REQUEST[$str2]);
				$dist = $dist[2]+$dist[3];
			  }
			  // }}}
			} elseif(preg_match("/\+.*\+/", $_REQUEST[$str2])) {
			  // {{{ Front single, back tripple
			  // EX: 3750+1360+1310
			  $dist = preg_split("/\+/", $_REQUEST[$str2]);
			  $dist = $dist[1]+$dist[2];
			  // }}}
			} elseif(preg_match("/\+/", $_REQUEST[$str2])) {
			  // {{{ Front single, back boggie
			  $dist = preg_split("/\+/", $_REQUEST[$str2]);
			  $dist = $dist[1];
			  // }}}
			} else {
			  // {{{ Front single, back single
			  $dist = $_REQUEST[$str2];
			  // }}}
			}

			eval("\$dist = ".htmlspecialchars($dist).";");
			if($DEBUG >= 2) {
			  echo "&nbsp;&nbsp;dist: ".$_REQUEST[$str2]."=$dist<br>";
			  echo "parse_table(TABLE_DATA[$axle], $dist, bk$bk)<br>";
			}
			// }}}

			$tmp = parse_table($TABLE_DATA[$axle], $dist, $bk);
			if($DEBUG >= 2)
			  echo "&nbsp;&nbsp;load=$tmp<br>";
			
			if($DEBUG >= 3)
			  echo "&nbsp;&nbsp;$axle axle (bk$bk / $type): load ($tmp) <= _REQUEST[$str] (".$_REQUEST[$str].")<br>";
			
			($tmp <= $_REQUEST[$str]) ? $$load = $tmp : $$load = $_REQUEST[$str];
			// }}}
		  }
		} else
		  $$load  = 0;

		if($DEBUG >= 2)
		  echo "Load $location (bk$bk / $type): '".$$load."'<p>";
	  }
	  // }}}

	  if($load_front and $load_back) {
		if($DEBUG >= 3)
		  echo "MAX_AXLE[$bk][$type] = '$load_front + $load_back = ".($load_front + $load_back)."'<br>";
		elseif(($DEBUG >= 1) and ($DEBUG <= 2))
		  echo "<br>";
		$MAX_AXLE[$bk][$type] = sprintf("%01.0f", ($load_front + $load_back));
	  } elseif($DEBUG >= 2) {
		echo "&nbsp;&nbsp;n/a<br>";
		$MAX_AXLE[$bk][$type] = 0;
	  }

	  if($DEBUG >= 3)
		echo "<br>";
	}

	if($DEBUG >= 2)
	  echo "--------------<p>";
  }

  if($DEBUG)
	printr($MAX_AXLE);
  // }}}

  // {{{ 4. $LOAD
  // Get the lowest denominator of GROSS_BK / MAX_AXLE
  if($DEBUG)
	echo "<b>Max Gross weight (\$LOAD)</b>:<br>";

  // {{{ Get number of axles
  foreach(array("truck", "trailer") as $type) {
	$NR_AXLES[$type] = 0;
  
	foreach(array("front", "back") as $location) {
	  $key = $type."_axles_".$location;

	  if($_REQUEST[$key] == 'single')
		$NR_AXLES[$type] += 1;
	  if($_REQUEST[$key] == 'boggie')
		$NR_AXLES[$type] += 2;
	  if($_REQUEST[$key] == 'tripple')
		$NR_AXLES[$type] += 3;
	}

	if($DEBUG >= 2)
	  echo "Number of axles on $type: '".$NR_AXLES[$type]."'<br>";
  }

  if($DEBUG >= 3)
	echo "<br>";
  // }}}

  for($bk = 1; $bk <= 3; $bk++) {
	foreach(array("truck", "trailer") as $type) {
	  if($DEBUG >= 3)
		echo "GROSS_BK[$bk][$type] (".$GROSS_BK[$bk][$type].") <= MAX_AXLE[$bk][$type] (".$MAX_AXLE[$bk][$type].")<br>";
	  ($GROSS_BK[$bk][$type] <= $MAX_AXLE[$bk][$type]) ? $LOAD[$bk][$type] = $GROSS_BK[$bk][$type] : $LOAD[$bk][$type] = $MAX_AXLE[$bk][$type];

	  // {{{ Check for common limitations...
	  if($LOAD[$bk][$type]) {
		$key = $type."_road_nice";
		
		if(($NR_AXLES[$type] == 2) and ($LOAD[$bk][$type] > 18))
		  // Two axles
		  $LOAD[$bk][$type] = 18;
		
		elseif(($NR_AXLES[$type] == 3) and !$_REQUEST[$key] and ($LOAD[$bk][$type] > 25))
		  // Three axles, Not road nice
		  $LOAD[$bk][$type] = 25;
		
		elseif(($NR_AXLES[$type] == 3) and  $_REQUEST[$key] and ($LOAD[$bk][$type] > 26))
		  // Three axles, road nice
		  $LOAD[$bk][$type] = 26;
		
		elseif(($NR_AXLES[$type] >= 4) and !$_REQUEST[$key] and ($LOAD[$bk][$type] > 31))
		  // >= Four axles, Not road nice
		  $LOAD[$bk][$type] = 31;
		
		elseif(($NR_AXLES[$type] >= 4) and  $_REQUEST[$key] and ($LOAD[$bk][$type] > 32))
		  // >= Four axles, road nice
		  $LOAD[$bk][$type] = 32;
	  } else
		$LOAD[$bk][$type] = 0;

	  $LOAD[$bk][$type] = sprintf("%01.02f", $LOAD[$bk][$type]);
	  if($DEBUG >= 3)
		echo "&nbsp;&nbsp;LOAD[$bk][$type] => '".$LOAD[$bk][$type]."'<br>";
	  // }}}
	}

	// c. train
	if($TRAILER) {
	  // Trailer - no front axle!

	  // Get distance from frontaxle, truck to rear backaxle, trailer
	  $dist = $_REQUEST["truck_link"] + $_REQUEST["trailer_link"];

	  $LOAD[$bk]["train"] = parse_table($TABLE_DATA[$bk], $dist, $bk);
	  if($DEBUG)
		echo "&nbsp;&nbsp;Train load (trailer, w/o front axle): '".$LOAD[$bk]["train"]."'<br>";
	} else {
	  $LOAD[$bk]["train"] = $LOAD[$bk]["truck"] + $LOAD[$bk]["trailer"];
	  if($DEBUG)
		echo "&nbsp;&nbsp;Train load (trailer, w/ front axle): '".$LOAD[$bk]["train"]."'<br>";
	}

	if($DEBUG >= 3)
	  echo "<br>";
  }

  if($DEBUG)
	printr($LOAD);
  // }}}


  // {{{ 5. Present results
?>
<html>
  <body>
    <table>
      <tr bgcolor="grey">
        <td colspan="3">Maximal bruttovikt, <?php
   if($_REQUEST["truck_link"] and $_REQUEST["trailer_link"]) {
	 echo "t&aring;g";
   } elseif(!$_REQUEST["truck_weight"] and  $_REQUEST["trailer_weight"]) {
	echo "sl&auml;p";
   } elseif( $_REQUEST["truck_weight"] and !$_REQUEST["trailer_weight"]) {
	echo "bil";
   } ?></td>
      </tr>

<?php
	for($bk = 1; $bk <= 3; $bk++) {
?>
      <tr>
        <td>BK<?php echo "$bk"; ?></td>
        <td><?php echo $LOAD[$bk]["train"]?> ton</td>
        <td>(<?php
			 $max_load_truck = $LOAD[$bk]["truck"] -  $_REQUEST["truck_weight"];
			 $max_load_train = ($LOAD[$bk]["train"] - ($_REQUEST["truck_weight"] + $_REQUEST["trailer_weight"]));
			 $max_load_trail = $max_load_train - $max_load_truck;

			 // Output...
			 $str  = "";
			 if($_REQUEST["truck_weight"] and $_REQUEST["trailer_weight"]) {
			   $str .= $max_load_truck . " + ";
			   $str .= $max_load_trail . " = ";
			 }

			 $str .= "<b>".$max_load_train."</b>";
			 echo "$str";
?> ton)
        </td>
      </tr>
<?php
	}
?>
    </table>
  </body>
</html>
<?php
  // }}}
}

// {{{ Copyleft etc
echo "<p><p><font size=\"-1\">Lastber&auml;rkningsprogram, Copyleft<br>Turbo Fredriksson &lt;turbo@bayour.com&gt;<br>";
echo "Version".preg_replace("/.*sion: ([0-9].*[0-9]) \$/", "\1", $VERSION);
echo "</font>";

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
// }}}
?>
