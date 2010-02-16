<?php
$DEBUG = 0;

// Calculate max load and gross load on truck and/or trailer
// $Id: index.php,v 1.5 2010-02-16 22:37:42 turbo Exp $

$VERSION = "$Revision: 1.5 $";

// {{{ Defines
$BK["1"]["FRONT"] = 10000;
$BK["1"]["BACK"]  = 11500;
$BK["2"]["FRONT"] = 10000;
$BK["2"]["BACK"]  = 10000;
$BK["3"]["FRONT"] =  8000;
$BK["3"]["BACK"]  =  8000;
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
function parse_table($data, $distance) {
  global $DEBUG;

  // Tables is in meters, but form is in milimeter...
  $d = $distance / 1000;

  for($i = 0; $data[$i]; $i++) {
	// Split up the line in three data parts
	// Ex:	11.00<11.25=44.00
	$vals = preg_split("/[\<\>\=]/", $data[$i]);

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
		// TODO
		die("Sorry, not yet implemented!<br>");
	  } else {
		if( (($d >= $vals[0]) and ($d < $vals[1])) or
			(($d >= $vals[0]) and ($vals[1] == '0')) )
		  return($vals[2]);
	  }
	} elseif($chars[6] == '>') {
	  // BK1 road with axle distance >= 18.00 meters
	  // Ex:	18.00>00.00=60.00
	  if( $d >= $vals[0] )
		return($vals[2]);
	}
  }
}
// }}}

if(!$_REQUEST["action"]) {
  // {{{ Input form
?>
<html>
  <body>
    <table>
      <form method="post" name="berakning":
        <tr>
          <td colspan="3"><h1>Bil:</h1></td>
        </tr>

        <tr>
	      <td>Tj&auml;nstevikt</td>
          <td><input type="text" name="truck_weight"></td>
          <td>Kg</td>
          <td><input type="checkbox" name="truck_weight_save"<?php if($_REQUEST["truck_weight_save"]) { echo " CHECKED"; } ?></td>
        </tr>

        <tr>
          <td>Max belastning<br>Framaxel</td>
          <td><input type="text" name="truck_load_front"></td>
          <td>Kg</td>
          <td><input type="checkbox" name="truck_load_front_save"></td>
        </tr>

        <tr>
          <td>Max belastning<br>Bakaxel</td>
          <td><input type="text" name="truck_load_back"></td>
          <td>Kg</td>
          <td><input type="checkbox" name="truck_load_back_save"></td>
        </tr>

        <tr>
          <td>Axelavst&aring;nd</td>
          <td><input type="text" name="truck_axle"></td>
          <td>mm</td>
          <td><input type="checkbox" name="truck_axle_save"></td>
        </tr>

        <tr>
          <td>Kopplingsavst&aring;nd</td>
          <td><input type="text" name="truck_link"></td>
          <td>mm</td>
          <td><input type="checkbox" name="truck_link_save"></td>
        </tr>

        <tr>
          <td></td>
          <td colspan="2"><font size="-2">* Endast n&ouml;dv&auml;ndig om fordonst&aring;g ber&auml;knas</font></td>
        </tr>

        <!-- ---------------------------------- -->

        <tr>
          <td colspan="3"><h1>Sl&auml;p:</h1></td>
        </tr>

        <tr>
          <td>Tj&auml;nstevikt</td>
          <td><input type="text" name="trailer_weight"></td>
          <td>Kg</td>
          <td><input type="checkbox" name="trailer_weight_save"<?php if($_REQUEST["trailer_weight_save"]) { echo " CHECKED"; } ?></td>
        </tr>

        <tr>
          <td>Max belastning<br>Framaxel</td>
          <td><input type="text" name="trailer_load_front"></td>
          <td>Kg</td>
          <td><input type="checkbox" name="trailer_load_front_save"></td>
        </tr>

        <tr>
          <td>Max belastning<br>Bakaxel</td>
          <td><input type="text" name="trailer_load_back"></td>
          <td>Kg</td>
          <td><input type="checkbox" name="trailer_load_back_save"></td>
        </tr>

        <tr>
          <td>Axelavst&aring;nd</td>
          <td><input type="text" name="trailer_axle"></td>
          <td>mm</td>
          <td><input type="checkbox" name="trailer_axle_save"></td>
        </tr>

        <tr>
          <td>Kopplingsavst&aring;nd</td>
          <td><input type="text" name="trailer_link"></td>
          <td>mm</td>
          <td><input type="checkbox" name="trailer_link_save"></td>
        </tr>

        <tr>
          <td></td>
          <td colspan="2"><font size="-2">* Endast n&ouml;dv&auml;ndig om fordonst&aring;g ber&auml;knas</font></td>
        </tr>
 
        <!-- ---------------------------------- -->

       <tr>
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
	echo "<p>";
  }
  // }}}

  // {{{ 1. Load BKx table file(s)
  for($bk = 1; $bk <= 3; $bk++) {
	$name = "table-bk".$bk.".txt";
	$fh = fopen($name, 'r') or die("Kan inte öppna bruttoviktstabell filerna!<br>");

	$j = 0;
	while($line = fgets($fh)) {
	  $TABLE_DATA[$bk][$j] = trim($line);

	  $j++;
	}

	fclose($fh);
  }

  if($DEBUG) {
	echo "<b>BK table data (\$TABLE_DATA)</b>:";
	printr($TABLE_DATA);
  }
  // }}}

  // {{{ 2. Get BKx table value dep. on axle distance
  for($bk = 1; $bk <= 3; $bk++) {
	// ab. truck and trailer
	foreach(array("truck", "trailer") as $type) {
	  $str = $type."_axle";
	  if($_REQUEST[$str])
		$GROSS_BK[$bk][$type] = parse_table($TABLE_DATA[$bk], $_REQUEST[$str]);
	}

	// c. train
	if($_REQUEST["truck_link"] and $_REQUEST["trailer_link"]) 
	  $GROSS_BK[$bk]["train"] = parse_table($TABLE_DATA[$bk], ($_REQUEST["truck_link"] + $_REQUEST["trailer_link"]));
  }	

  if($DEBUG) {
	echo "<b>Gross BK values (\$GROSS_BK)</b>:";
	printr($GROSS_BK);
  }
  // }}}

  // {{{ 3. Calculate max axle load
  foreach(array("truck", "trailer") as $type) {
	for($bk = 1; $bk <= 3; $bk++) {
	  // Get the lowest axle load allowed - front
	  $str = $type.'_load_front';
	  if($_REQUEST[$str])
		($_REQUEST[$str] <= $BK[$bk]["FRONT"]) ? $load_front = $_REQUEST[$str] : $load_front = $BK[$bk]["FRONT"];
	  else
		$load_front = 0;

	  // Get the lowest axle load allowed - back
	  $str = $type.'_load_back';
	  if($_REQUEST[$str])
		($_REQUEST[$str]  <= $BK[$bk]["BACK"])  ? $load_back  = $_REQUEST[$str]  : $load_back  = $BK[$bk]["BACK"];
	  else
		$load_back  = 0;

	  $MAX_AXLE[$bk][$type] = sprintf("%01.02f", ($load_front + $load_back) / 1000);
	}
  }

  if($DEBUG) {
	echo "<b>Max axle load (\$MAX_AXLE)</b>";
	printr($MAX_AXLE);
  }
  // }}}

  // {{{ 4. Get the lowest denominator of GROSS_BK / MAX_AXLE
  for($bk = 1; $bk <= 3; $bk++) {
	foreach(array("truck", "trailer") as $type) {
	  ($GROSS_BK[$bk][$type] <= $MAX_AXLE[$bk][$type]) ? $LOAD[$bk][$type] = $GROSS_BK[$bk][$type] : $LOAD[$bk][$type] = $MAX_AXLE[$bk][$type];
	}

	// c. train
	$LOAD[$bk]["train"] = $LOAD[$bk]["truck"] + $LOAD[$bk]["trailer"];
  }

  if($DEBUG) {
	echo "<b>Max Gross weight (\$LOAD)</b>";
	printr($LOAD);
  }
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
			 $max_load_truck = ((($LOAD[$bk]["truck"] * 1000) - $_REQUEST["truck_weight"]) / 1000);
			 $max_load_train = ((($LOAD[$bk]["train"] * 1000 ) - ($_REQUEST["truck_weight"] + $_REQUEST["trailer_weight"])) / 1000);
			 $max_load_trail = $max_load_train - $max_load_truck;

			 echo $max_load_truck." + ".$max_load_trail." = <b>".$max_load_train."</b>"; ?> ton)</td>
			 
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
?>
