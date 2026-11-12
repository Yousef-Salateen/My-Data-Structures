#pragma once
#include <stack>
#include <string>

class clsTextHolder
{
private:
	std::stack<std::string> _Undo;
	std::stack<std::string> _Redo;
	std::string _Text;

public:
	clsTextHolder() : _Text("")
	{

	}

	clsTextHolder(const std::string& text) : _Text(text)
	{

	}

	void setText(const std::string& text)
	{
		_Undo.push(_Text);
		_Text = text;
	}

	inline const std::string& Text() const
	{
		return _Text;
	}

	void Undo()
	{
		if (_Undo.empty()) return;
		_Redo.push(_Text);
		_Text = _Undo.top();
		_Undo.pop();
	}

	void Redo()
	{
		if (_Redo.empty()) return;
		_Undo.push(_Text);
		_Text = _Redo.top();
		_Redo.pop();
	}
};

