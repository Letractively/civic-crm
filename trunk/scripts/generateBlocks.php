<?php
/**
 * @copyright 2006-2009 City of Bloomington, Indiana
 * @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.txt
 * @author Cliff Ingham <inghamn@bloomington.in.gov>
 */
include '../configuration.inc';
$zend_db = Database::getConnection();

foreach ($zend_db->listTables() as $tableName) {
	$fields = array();
	$primary_keys = array();
	foreach ($zend_db->describeTable($tableName) as $row) {
		$type = preg_replace("/[^a-z]/","",strtolower($row['DATA_TYPE']));

		// Translate database datatypes into PHP datatypes
		if (preg_match('/int/',$type)) {
			$type = 'int';
		}
		if (preg_match('/enum/',$type) || preg_match('/varchar/',$type)) {
			$type = 'string';
		}

		$fields[] = array('field'=>$row['COLUMN_NAME'],'type'=>$type);

		if ($row['PRIMARY']) {
			$primary_keys[] = $row['COLUMN_NAME'];
		}
	}

	// Only generate code for tables that have a single-column primary key
	// Code for other tables will need to be created by hand
	if (count($primary_keys) != 1) {
		continue;
	}
	$key = $primary_keys[0];

	$tableName = strtolower($tableName);
	$className = Inflector::classify($tableName);
	$variableName = Inflector::singularize($tableName);
	$acl_resource = ucfirst($tableName);

	/**
	 * Generate the list block
	 */
	$getId = "get".ucwords($key);
	$HTML = "<?php\n";
	$HTML.= COPYRIGHT;
	$HTML.= "
?>
<div class=\"interfaceBox\">
	<h1>
		<?php
			if (userIsAllowed('$acl_resource')) {
				echo \$this->template->linkButton(
					'Add',BASE_URL.'/$tableName/update$className.php','add'
				);
			}
		?>
		{$className}s
	</h1>
	<ul><?php
			foreach (\$this->{$variableName}List as \${$variableName}) {
				\$editButton = '';
				if (userIsAllowed('$acl_resource')) {
					\$url = new URL(BASE_URL.'/$tableName/update$className.php');
					\$url->$key = \${$variableName}->{$getId}();

					\$editButton = \$this->template->linkButton(
						'Edit',
						BASE_URL.'/$tableName/update$className.php?$key='.\${$variableName}->{$getId}(),
						'edit'
					);
				}
				echo \"<li>\$editButton \$$variableName</li>\";
			}
		?>
	</ul>
</div>";


	$dir = APPLICATION_HOME."/scripts/stubs/blocks/$tableName";
	if (!is_dir($dir)) {
		mkdir($dir,0770,true);
	}
	file_put_contents("$dir/{$variableName}List.inc",$HTML);

/**
 * Generate the Update Form
 */
$HTML = "<?php\n";
$HTML.= COPYRIGHT;
$HTML.="
\$title = \$this->{$variableName}->getId() ? 'Edit $className' : 'Add $className';
?>
<h1><?php echo \$title; ?></h1>
<form method=\"post\" action=\"<?php echo \$_SERVER['SCRIPT_NAME']; ?>\">
	<fieldset><legend>$className Info</legend>
		<input name=\"$key\" type=\"hidden\" value=\"<?php echo \$this->{$variableName}->{$getId}(); ?>\" />
		<table>
";
		foreach ($fields as $field) {
			if ($field['field'] != $key) {
				$fieldFunctionName = ucwords($field['field']);
				switch ($field['type']) {
					case 'date':
					$HTML.="
			<tr><td><label for=\"$field[field]-mon\">$field[field]</label></td>
				<td><select name=\"$field[field][mon]\" id=\"$field[field]-mon\">
						<option></option>
						<?php
							\$$field[field] = \$this->{$variableName}->dateStringToArray(\$this->{$variableName}->get$fieldFunctionName());
							for (\$i=1; \$i<=12; \$i++) {
								\$selected = (\$i==\$$field[field]['mon']) ? 'selected=\"selected\"' : '';
								echo \"<option \$selected>\$i</option>\";
							}
						?>
					</select>
					<select name=\"$field[field][mday]\">
						<option></option>
						<?php
							for (\$i=1; \$i<=31; \$i++) {
								\$selected = (\$i==\$$field[field]['mday']) ? 'selected=\"selected\"' : '';
								echo \"<option \$selected>\$i</option>\";
							}
						?>
					</select>
					<input name=\"$field[field][year]\" id=\"$field[field]-year\" size=\"4\" maxlength=\"4\" value=\"<?php echo \$$field[field]['year']; ?>\" />
				</td>
			</tr>";
						break;

					case 'datetime':
					case 'timestamp':
					$HTML.="
			<tr><td><label for=\"$field[field]-mon\">$field[field]</label></td>
				<td><select name=\"$field[field][mon]\" id=\"$field[field]-mon\">
						<option></option>
						<?php
							\$$field[field] = \$this->{$variableName}->dateStringToArray(\$this->{$variableName}->get$fieldFunctionName());
							for (\$i=1; \$i<=12; \$i++) {
								\$selected = (\$i==\$$field[field]['mon']) ? 'selected=\"selected\"' : '';
								echo \"<option \$selected>\$i</option>\";
							}
						?>
					</select>
					<select name=\"$field[field][mday]\">
						<option></option>
						<?php
							for (\$i=1; \$i<=31; \$i++) {
								\$selected = (\$i==\$$field[field]['mday']) ? 'selected=\"selected\"' : '';
								echo \"<option \$selected>\$i</option>\";
							}
						?>
					</select>
					<input name=\"$field[field][year]\" id=\"$field[field]-year\" size=\"4\" maxlength=\"4\" value=\"<?php echo \$$field[field]['year']; ?>\" />
					<select name=\"$field[field][hours]\" id=\"$field[field]-hours\">
					<?php
						for (\$i=0; \$i<=23; \$i++) {
							\$selected = (\$i==\$$field[field]['hours']) ? 'selected=\"selected\"' : '';
							echo \"<option \$selected>\$i</option>\";
						}
					?>
					</select>
					<select name=\"$field[field][minutes]\" id=\"$field[field]-minutes\">
					<?php
						for (\$i=0; \$i<=59; \$i+=15) {
							\$selected = (\$i==\$$field[field]['minutes']) ? 'selected=\"selected\"' : '';
							echo \"<option \$selected>\$i</option>\";
						}
					?>
					</select>
				</td>
			</tr>";
						break;

					case 'text':
				$HTML.= "
			<tr><td><label for=\"$field[field]\">$field[field]</label></td>
				<td><textarea name=\"$field[field]\" id=\"$field[field]\" rows=\"3\" cols=\"60\"><?php echo \$this->{$variableName}->get$fieldFunctionName(); ?></textarea>
				</td>
			</tr>
				";
						break;

					default:
				$HTML.= "
			<tr><td><label for=\"$field[field]\">$field[field]</label></td>
				<td><input name=\"$field[field]\" id=\"$field[field]\" value=\"<?php echo \$this->{$variableName}->get$fieldFunctionName(); ?>\" />
				</td>
			</tr>
				";
				}
			}
		}
	$HTML.= "
		</table>
		<?php
			echo \$this->template->formButton('Submit','submit','submit');
			echo \$this->template->formButton(
				'Cancel','button','cancel',null,\"document.location.href='\".BASE_URL.\"/{$variableName}s';\"
			);
		?>
	</fieldset>
</form>";
file_put_contents("$dir/update{$className}Form.inc",$HTML);

echo "$className\n";
}