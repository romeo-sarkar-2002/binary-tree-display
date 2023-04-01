#ifndef LAYOUT_C
#define LAYOUT_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "error_internal.h"
// enums
typedef enum _TypeNode { null_ = 0, not_null_ } _TypeNode;
typedef enum _TypeChild { left_child_ = -1, root_, right_child_ } _TypeChild;

typedef uint32_t uint;

// declaration
typedef struct Layout Layout;

Layout *layout_generate(const void *_node);
void    layout_destroy_recursive(Layout *_l);

void    layout_print(const Layout *_layout);
void    layout_traverse_inorder(const Layout *_l);

int     layout_is_null(const Layout *_layout);

const Layout *layout_get_parent(const Layout *_layout);
const Layout *layout_get_left_child(const Layout *_layout);
const Layout *layout_get_right_child(const Layout *_layout);



typedef struct Layout
{
    char    *_str;
    
    _TypeChild  _c;
    _TypeNode   _n;
    
    uint    _level;
    
    uint    _str_offset;
    uint    _str_width;
    uint    _str_width_left;
    uint    _str_width_right;

    uint    _tree_offset;
    uint    _tree_width;

    const struct Layout         *_parent;
    struct Layout           *_left_child;
    struct Layout          *_right_child;
} Layout;

#define TAB "  "
void layout_print(const Layout *_layout)
{
    ERROR(layout_print, 0 == _layout, "_layout is (null)", return);
    printf("\e[31m\"\e[1;4;33m%s\e[0;31m\"\e[33m:\e[m", _layout->_str);
    switch(_layout->_c)
    {
        case left_child_:
            printf(TAB "\e[1;4mleft\e[m child, ");
            break;
        case root_:
            printf(TAB "\e[1;4mroot\e[m, ");
            break;
        case right_child_:
            printf(TAB "\e[1;4mright\e[m child, ");
    }
    switch(_layout->_n)
    {
        case null_:
            printf("\e[1;90mnull\e[m\n");
            break;
        case not_null_:
            printf("\e[1;33mnot_null\e[m\n");
    }
    
    printf(TAB "\e[4mlevel:\e[m \e[1;32m%u\e[m\n", _layout->_level);

    printf(TAB "_str_offset: %u\n", _layout->_str_offset);
    printf(TAB "_str_width: %u\n", _layout->_str_width);
    printf(TAB "_str_width_left: %u\n", _layout->_str_width_left);
    printf(TAB "_str_width_right: %u\n", _layout->_str_width_right);

    printf(TAB "_tree_offset: %u\n", _layout->_tree_offset);
    printf(TAB "_tree_width: %u\n", _layout->_tree_width);

    if(_layout->_parent)
    {
        printf(TAB "_parent: %s\n", _layout->_parent->_str);
    }
    if(_layout->_left_child)
    {
        printf(TAB "_left_child: %s\n", _layout->_left_child->_str);
    }
    if(_layout->_right_child)
    {
        printf(TAB "_right_child: %s\n", _layout->_right_child->_str);
    }
}
#undef TAB

// function_pointers
static const void *(*func_get_left_child_)(const void *node);
static const void *(*func_get_right_child_)(const void *node);
static char       *(*func_get_string_)(const void *node);

void layout_set_func_get_left_child(const void *(*_func_get_left_child)(const void *node))
{
    func_get_left_child_ = _func_get_left_child;
}
void layout_set_func_get_right_child(const void *(*_func_get_right_child)(const void *node))
{
    func_get_right_child_ = _func_get_right_child;
}
void layout_set_func_get_string(char *(*_func_get_string)(const void *node))
{
    func_get_string_ = _func_get_string;
}


static int max(int a, int b)
{
    if(a > b)return a;
    return b;
}

uint layout_str_width(const char *_str)
{
    return strlen(_str);
}

char *layout_str_null()
{
    char *string = (char *)malloc(8);
    string[0] = '*';
    string[1] = '\0';
    return string;
}

static Layout *layout_create(const void *_node, _TypeChild _c, uint _level, uint _offset, Layout *_parent)
{
    Layout *l = (Layout *)malloc(sizeof(Layout));
    ERROR(layout_create, 0 == l, "malloc returned (null)", return 0);

    if(0 == _node)
    {
        l->_n = null_;
    }
    else
    {
        l->_n = not_null_;
    }
    l->_c = _c;

    l->_level = _level;

    l->_str = func_get_string_(_node);
    if(0 == l->_str || 0 == layout_str_width(l->_str))
    {
        l->_str = layout_str_null();
    }
    l->_str_width = layout_str_width(l->_str);
    l->_str_offset = _offset;
    // left_width:
    l->_str_width_left = (l->_str_width - 1) >> 1;
    if((0 == (l->_str_width & 1)) && _c == left_child_)    //     ╭── (left_child_)
    {                                                      //  acbdef   => [abc] + [d] + [ef]  => [abc] + d + ef
        l->_str_width_left++;                              //  ──╮ (right_child_)
    }                                                      //  abcdef   => [ab] + [c] + [def]  => [ab] + c + def
    // right_width:
    l->_str_width_right = ((l->_str_width - 1) >> 1);
    if((0 == (l->_str_width & 1)) && _c == right_child_)    //     ╭── (left_child_)
    {                                                       //  acbdef   => [abc] + [d] + [ef]  => abc + d + [ef]
        l->_str_width_right++;                              //  ──╮ (right_child_)
    }                                                       //  abcdef   => [ab] + [c] + [def]  => ab + c + [def]
    
    l->_tree_offset = 0;
    l->_tree_width = l->_str_width;

    l->_left_child = l->_right_child = 0;
    l->_parent = _parent;

    return l;
}

static Layout *layout_generate_(const void *_node, _TypeChild _flag, uint _level, uint _offset, Layout *_parent)
{
    Layout *l = layout_create(_node, _flag, _level, _offset, _parent);

    if(0 == _node)
    {
        return l;
    }
    // left child
    l->_left_child = layout_generate_(func_get_left_child_(_node), left_child_, 1 + _level, _offset, l);
    // right child
    l->_right_child = layout_generate_(func_get_right_child_(_node), right_child_, 1 + _level, _offset + l->_left_child->_tree_width + 1, l);
    // width calculations =>
    int max_width_left;
    // left_width:
    {
        int subtree_width_left = l->_left_child->_tree_width;
        int str_width_left = l->_str_width_left;

        l->_str_width_left = str_width_left;

        if(subtree_width_left < str_width_left)
        { // offset the left subtree by required amount
            max_width_left = str_width_left;
            l->_left_child->_tree_offset = l->_right_child->_tree_offset = str_width_left - subtree_width_left;
        }
        else // str_width_left < subtree_width_left
        {
            max_width_left = subtree_width_left;
            l->_str_offset += subtree_width_left - str_width_left;
        }
    }
    int max_width_right;
    // right_width:
    {
        int subtree_width_right = l->_right_child->_tree_width;
        int str_width_right = l->_str_width_right;

        max_width_right = max(subtree_width_right, str_width_right);
    }
    
    // setting the width value =>
    l->_tree_width = max_width_left + max_width_right + 1;

    return l;
}

static void layout_recompute_offset(struct Layout *_l, uint _offset)
{
    _offset += _l->_tree_offset;
    _l->_str_offset += _offset;
    _l->_tree_offset = 0;
    if(_l->_n == not_null_)
    {
        layout_recompute_offset(_l->_left_child, _offset);
        layout_recompute_offset(_l->_right_child, _offset);
    }
}

void layout_traverse_inorder(const Layout *_l)
{
    if(_l->_n == not_null_)
    {
        layout_traverse_inorder(_l->_left_child);
    }
    layout_print(_l);
    
    if(_l->_n == not_null_)
    {
        layout_traverse_inorder(_l->_right_child);
    }
}

static void layout_destroy(Layout *_l)
{
    if(0 == _l)
    {
        return;
    }
    if(0 != _l->_str)
    {
        free(_l->_str);
    }
    free(_l);
}

void layout_destroy_recursive(Layout *_l)
{
    if(0 == _l)
    {
        return;
    }
    if(_l->_n == not_null_)
    {
        layout_destroy_recursive(_l->_left_child);
        layout_destroy_recursive(_l->_right_child);
    }
    layout_destroy(_l);
}

Layout *layout_generate(const void *_node)
{
    ERROR(layout_generate, 0 == _node, "_node is (null)", return 0);
    Layout *l = layout_generate_(_node, root_, 0, 0, 0);
    layout_recompute_offset(l, 0);
    return l;
}

int layout_is_null(const Layout *_layout)
{
    if(null_ == _layout->_n)
    {
        return 1;
    }
    return 0;
}

const Layout *layout_get_parent(const Layout *_layout)
{
    return _layout->_parent;
}
const Layout *layout_get_left_child(const Layout *_layout)
{
    return _layout->_left_child;
}
const Layout *layout_get_right_child(const Layout *_layout)
{
    return _layout->_right_child;
}

#endif