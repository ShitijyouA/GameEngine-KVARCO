#pragma once

namespace kvarco
{
namespace tag
{

struct ID		{};
struct Z_depth	{};
struct Type		{};
struct Path		{};
struct Sequence	{};
struct Name		{};
struct Handle	{};

#define TO_TAG(x) boost::multi_index::tag<x>
namespace detail
{
typedef TO_TAG(ID)			ID;
typedef TO_TAG(Z_depth)		Z_depth;
typedef TO_TAG(Type)		Type;
typedef TO_TAG(Path)		Path;
typedef TO_TAG(Sequence)	Sequence;
typedef TO_TAG(Name)		Name;
typedef TO_TAG(Handle)		Handle;
}
#undef TO_TAG

}
}