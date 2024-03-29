<?php
/**
 * @copyright 2011 City of Bloomington, Indiana
 * @license http://www.gnu.org/licenses/agpl.txt GNU/AGPL, see LICENSE.txt
 * @author Cliff Ingham <inghamn@bloomington.in.gov>
 * @param Category $this->category
 */
$title = $this->category->getId() ? 'Edit Category' : 'Add Category';

$fields = array(
	'name', 'description', 'postingPermissionLevel', 'displayPermissionLevel',
	'categoryGroup_id', 'department_id'
);
foreach ($fields as $field) {
	$get = 'get'.ucfirst($field);
	$$field = isset($_POST[$field]) ? View::escape($_POST[$field]) : View::escape($this->category->$get());
}

$custom_fields = isset($_POST['custom_fields'])
	? View::escape($_POST['custom_fields'])
	: json_encode($this->category->getCustomFields());
?>
<h1><?php echo $title; ?></h1>
<form method="post" action="<?php echo BASE_URL; ?>/categories/update">
	<fieldset><legend>Category</legend>
		<input name="category_id" type="hidden" value="<?php echo $this->category->getId(); ?>" />
		<table>
			<tr><td><label for="name" class="required">Name</label></td>
				<td><input name="name" id="name" value="<?php echo $name; ?>" /></td>
			</tr>
			<tr><td><label for="categoryGroup_id">Group</label></td>
				<td><select name="categoryGroup_id" id="categoryGroup_id">
					<?php
						$groups = new CategoryGroupList();
						$groups->find();
						foreach ($groups as $group) {
							$selected = $categoryGroup_id==$group->getId()
								? 'selected="selected"'
								: '';
							$name = View::escape($group->getName());
							echo "<option value=\"{$group->getId()}\" $selected>$name</option>";
						}
					?>
					</select>
				</td>
			</tr>
		</table>
		<div>
			<div><label for="description">Description</label></div>
			<textarea name="description" id="description" rows="3" cols="50"><?php echo $description; ?></textarea>
		</div>
		<div>
			<div><label for="department_id">Default Department Assignment</label></div>
			<select name="department_id" id="department_id">
			<?php
				$list = new DepartmentList();
				$list->find();
				foreach ($list as $department) {
					$selected = $department->getId()==$department_id
						? 'selected="selected"'
						: '';
					$name = View::escape($department->getName());
					echo "<option value=\"{$department->getId()}\" $selected >$name</option>";
				}
			?>
			</select>
		</div>
		<div>
			<div><label for="postingPermissionLevel">Permission Level for Posting Cases</label></div>
			<select name="postingPermissionLevel" id="postingPermissionLevel">
				<option value="staff" <?php if ($postingPermissionLevel=='staff') echo 'selected="selected"'; ?> >
					Authenticated Staff
				</option>
				<option value="public" <?php if ($postingPermissionLevel=='public') echo 'selected="selected"'; ?> >
					Authenticated Public
				</option>
				<option value="anonymous" <?php if ($postingPermissionLevel=='anonymous') echo 'selected="selected"'; ?> >
					Anonymous
				</option>
			</select>
		</div>
		<div>
			<div><label for="displayPermissionLevel">Permission Level for Displaying Cases</label></div>
			<select name="displayPermissionLevel" id="displayPermissionLevel">
				<option value="staff" <?php if ($displayPermissionLevel=='staff') echo 'selected="selected"'; ?> >
					Authenticated Staff
				</option>
				<option value="public" <?php if ($displayPermissionLevel=='public') echo 'selected="selected"'; ?> >
					Authenticated Public
				</option>
				<option value="anonymous" <?php if ($displayPermissionLevel=='anonymous') echo 'selected="selected"'; ?> >
					Anonymous
				</option>
			</select>
		</div>
		<div>
			<div><label for="custom_fields">Custom Fields</label></div>
			<textarea name="custom_fields" id="custom_fields" rows="30" cols="50"><?php echo $custom_fields; ?></textarea>
			<div><p>Custom fields are defined in JSON.
					Each field can have a name, label, type and values.
					Each field requires at least a name.
				</p>
				<h3>Type</h3>
				<p>The currently supported types are taken from the Open311 spec.
					<a href="http://wiki.open311.org/GeoReport_v2#GET_Service_Definition">
						http://wiki.open311.org/GeoReport_v2#GET_Service_Definition
					</a>
				</p>
				<ul><li>string: A string of characters without line breaks. Represented in an HTML from using an &lt;input&gt; tag</li>
					<!--<li>number: A numeric value. Represented in an HTML from using an &lt;input&gt; tag</li> -->
					<!--<li>datetime: The input generated must be able to transform into a valid ISO 8601 date. Represented in an HTML from using &lt;input&gt; tags</li> -->
					<li>text:  A string of characters that may contain line breaks. Represented in an HTML from using an &lt;textarea&gt; tag</li>
					<li>singlevaluelist:  A set of predefined values (specified in this response) where only one value may be selected. Represented in an HTML from using the &lt;select&gt; and &lt;option&gt; tags</li>
					<li>multivaluelist:  A set of predefined values (specified in this response) where several values may be selected. Represented in an HTML from using the &lt;select multiple="multiple"&gt; and &lt;option&gt; tags</li>
				</ul>
				<p>
					If the type is not provided, or an unknown type is used, it will be treated as "string".
				</p>
				<h3>Values</h3>
				<p>Values are the list of predefined options for singlevaluelist and multivaluelist.
					Values are required if type is set to singlevaluelist or multivaluelist.
				</p>
				<h3>Example</h3>
				<code><pre>[
	{"name":"one","label":"Default"},
	{"name":"two","label":"Single Value List","type":"singlevaluelist","values":["a","b","c"]},
	{"name":"three","label":"Multi Value List","type":"multivaluelist","values":["d","e","f","g"]},
	{"name":"four","label":"Text Box","type":"text"}
]
				</pre></code>
			</div>
		</div>
		<button type="submit" class="submit">Submit</button>
		<a class="cancel" href="<?php echo BASE_URL; ?>/categories">Cancel</a>
	</fieldset>
</form>
