#pragma once

/*
	In case of ANY modification on the enum class ColumnType, please do
	the necessary modifications on the following stuctures:
	
	1 - column.cpp -> setType()
	2 - utils.cpp  -> utilGnrl::columnTypeToStr()
*/

enum class ColumnType {
	Text,
	Number,
	Currency,
	EGN,
	FacultyNumber
};