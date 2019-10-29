def deep_copy(item):
    if type(item) == str:
        return item

    new_list = []

    for i in item:
        new_list.append(deep_copy(i))

    return new_list

def create_result_error(command, curr_dir):
    return ('ERROR', command, curr_dir)

def create_result_success(FS, curr_dir):
    return ('SUCCESS', FS, curr_dir)

def create_error(FS, curr_dir, command):
    return (False, FS, curr_dir, command)

def create_success(FS, curr_dir, command):
    return (True, FS, curr_dir, command)

def convert_path_string(path):
    return '/' + '/'.join(path)

def convert_string_absolute_path(curr_dir, relative_path_string):
    items = relative_path_string.split('/')

    if len(items) >= 1 and items[0] == '':
        curr_dir = []

    for i in range(len(items) - 1, -1, -1):
        if items[i] == '':
            del items[i]

    return convert_absolute_path(curr_dir, items)

def convert_absolute_path(curr_dir, relative_path):
    if len(relative_path) == 0:
        return curr_dir

    elif relative_path[0] == '' or relative_path[0] == '.':
        return convert_absolute_path(curr_dir, relative_path[1:])

    elif relative_path[0] == '..':
        if len(curr_dir) == 0: return None
        return convert_absolute_path(curr_dir[:len(curr_dir) - 1], relative_path[1:])

    return convert_absolute_path(curr_dir + [relative_path[0]], relative_path[1:])

def check_fs_is_file(FS):
    return False if FS is None else FS[1].lower() == 'f'

def check_fs_is_dir(FS):
    return False if FS is None else FS[1].lower() == 'd'

def check_fs_is_dir_and_empty(FS):
    return check_fs_is_dir(FS) and len(FS) == 2

def is_child_of(FS, path1, path2):
    if not check_path_is_dir(FS, path2) or len(path1) > len(path2):
        return False

    for i in range(len(path2)):
        if len(path1) <= i or path1[i] != path2[i]:
            return False

    return True

def search_path(FS, path):
    if len(path) == 0: # we are done
        return FS

    if check_fs_is_dir(FS):
        for sub_fs in FS[2:]:
            if sub_fs[0] == path[0]: # we found a match
                found_path = search_path(sub_fs, path[1:])

                if found_path is not None:
                    return found_path

    return None

def rm_item(FS, path):
    for i in range(len(FS) - 1, 1, -1): # dont worry, since FS starts with /, there is no possibiliy that deleting root
        if FS[i][0] == path[0]:
            if len(path) == 1:
                del FS[i]
            else:
                FS[i] = rm_item(FS[i], path[1:])

    return FS

def check_path_is_file(FS, check_path):
    path = search_path(FS, check_path)

    if path is None: return False

    return check_fs_is_file(path)

def check_path_is_dir(FS, check_path):
    path = search_path(FS, check_path)

    if path is None: return False

    return check_fs_is_dir(path)

def check_path_is_dir_and_empty(FS, path):
    path = search_path(FS, path)

    return path is not None and check_fs_is_dir_and_empty(path)

def try_command_cd(FS, curr_dir, command, parameters):
    if len(parameters) == 0: return create_success(FS, [], command)
    elif len(parameters) != 1: return create_error(FS, curr_dir, command)

    path = search_path(FS, parameters[0])

    if path is not None and check_fs_is_dir(path):
        return create_success(FS, parameters[0], command)

    return create_error(FS, curr_dir, command)

def try_command_rmdir(FS, curr_dir, command, parameters):
    if len(parameters) != 1: return create_error(FS, curr_dir, command)

    if search_path(FS, parameters[0]) is not None and check_path_is_dir_and_empty(FS, parameters[0]) and not is_child_of(FS, curr_dir, parameters[0]):
        return create_success(rm_item(FS, parameters[0]), curr_dir, command)

    return create_error(FS, curr_dir, command)

def try_command_rm(FS, curr_dir, command, parameters):
    if len(parameters) != 1: return create_error(FS, curr_dir, command)

    if search_path(FS, parameters[0]) is not None and check_path_is_file(FS, parameters[0]):
        return create_success(rm_item(FS, parameters[0]), curr_dir, command)

    return create_error(FS, curr_dir, command)

def try_command_mkdir(FS, curr_dir, command, parameters):
    if len(parameters) != 1: return create_error(FS, curr_dir, command)

    parent_dir = search_path(FS, parameters[0][:len(parameters[0]) - 1])

    if parent_dir is not None and search_path(FS, parameters[0]) is None:
        parent_dir.append([parameters[0][-1], 'd'])

        return create_success(FS, curr_dir, command)

    return create_error(FS, curr_dir, command)

def try_command_exec(FS, curr_dir, command, parameters):
    if len(parameters) != 1: return create_error(FS, curr_dir, command)

    if search_path(FS, parameters[0]) is not None and check_path_is_file(FS, parameters[0]):
        return create_success(FS, curr_dir, command)

    return create_error(FS, curr_dir, command)

def try_command_cp(FS, curr_dir, command, parameters):
    if len(parameters) != 2: return create_error(FS, curr_dir, command)

    source = search_path(FS, parameters[0])
    target = search_path(FS, parameters[1])

    if source is None: return create_error(FS, curr_dir, command)

    copy_source = deep_copy(source)

    if check_fs_is_file(source) and target is None:
        parent_target = search_path(FS, target[:len(target) - 1])

        if check_fs_is_dir(parent_target):
            parent_target.append(copy_source)

            return create_success(FS, curr_dir, command)

    elif check_fs_is_file(source) and check_fs_is_dir(target):
        target.append(copy_source)
        return create_success(FS, curr_dir, command)

    elif check_fs_is_dir(source) and target is None:
        parent_target = search_path(FS, parameters[1][:len(parameters[1]) - 1])

        if check_fs_is_dir(parent_target):
            copy_source[0] = parameters[1][-1]
            parent_target.append(copy_source)

            return create_success(FS, curr_dir, command)

    elif check_fs_is_dir(source) and check_fs_is_dir(target):
        override_target = search_path(FS, parameters[1] + [parameters[0][-1]])

        if override_target is None:
            target.append(copy_source)

            return create_success(FS, curr_dir, command)

    return create_error(FS, curr_dir, command)

def try_command(FS, curr_dir, command):
    command_items = command.split(' ')

    for i in range(len(command_items) - 1, -1, -1): # for cleaning empty items
        if command_items[i] == '':
            del command_items[i]

    if len(command_items) == 0: return create_error(FS, curr_dir, command) # since there is no non parameter command

    command_name = command_items[0]
    parameters = command_items[1:]

    for i in range(len(parameters)):
        parameters[i] = convert_string_absolute_path(curr_dir, parameters[i])

        if parameters[i] is None:
            return create_error(FS, curr_dir, command)

    if command_name == 'cd':
        return try_command_cd(FS, curr_dir, command, parameters)
    elif command_name == 'rmdir':
        return try_command_rmdir(FS, curr_dir, command, parameters)
    elif command_name == 'rm':
        return try_command_rm(FS, curr_dir, command, parameters)
    elif command_name == 'mkdir':
        return try_command_mkdir(FS, curr_dir, command, parameters)
    elif command_name == 'cp':
        return try_command_cp(FS, curr_dir, command, parameters)
    elif command_name == 'exec':
        return try_command_exec(FS, curr_dir, command, parameters)

    return create_error(FS, curr_dir, command)

def check_commands(FS, command_list):
    curr_dir = []

    for command in command_list:
        result = try_command(FS, curr_dir, command)

        if result[0] == True:
            FS = result[1]
            curr_dir = result[2]
        else:
            return create_result_error(command, convert_path_string(curr_dir))

    return create_result_success(FS, convert_path_string(curr_dir))