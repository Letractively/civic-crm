<?php
/**
 * @copyright 2011 City of Bloomington, Indiana
 * @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.txt
 * @author Cliff Ingham <inghamn@bloomington.in.gov>
 * @param DepartmentList $this->departmentList
 */
?>
<div class="interfaceBox">
	<h1><?php
			if (userIsAllowed('Departments')) {
				echo $this->template->linkButton(
					'Add',BASE_URL.'/departments/updateDepartment.php','add'
				);
			}
		?>
		Departments
	</h1>
	<table>
		<thead>
			<tr><th></th>
				<th>Name</th>
				<th>Default Person</th>
				<th>Categories</th>
				<th>Actions</th>
			</tr>
		</thead>
		<tbody>
		<?php
			foreach ($this->departmentList as $department) {
				$editButton = '';
				if (userIsAllowed('Departments')) {
					$url = new URL(BASE_URL.'/departments/updateDepartment.php');
					$url->id = $department->getId();

					$editButton = $this->template->linkButton(
						'Edit',
						BASE_URL.'/departments/updateDepartment.php?department_id='.$department->getId(),
						'edit'
					);
				}
				$name = View::escape($department->getName());
				$person = View::escape($department->getDefault_person()->getFullname());

				$categories = '';
				if ($department->hasCategories()) {
					$categories = '<ul>';
					foreach ($department->getCategories() as $category) {
						$c = View::escape($category);
						$categories.="<li>$c</li>";
					}
					$categories.='</ul>';
				}

				$actions = '';
				if ($department->hasActions()) {
					$actions = '<ul>';
					foreach ($department->getActions() as $action) {
						$a = View::escape($action->getName());
						$actions.="<li>$a</li>";
					}
					$actions.='</ul>';
				}
				echo "
				<tr><td>$editButton</td>
					<td>$name</td>
					<td>$person</td>
					<td>$categories</td>
					<td>$actions</td>
				</tr>
				";
			}
		?>
		</tbody>
	</table>
</div>