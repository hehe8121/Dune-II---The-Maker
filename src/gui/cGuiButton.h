#ifndef CGUIBUTTON_H_
#define CGUIBUTTON_H_

class cGuiButton : public cGuiShape{

	public:
		cGuiButton(cRectangle * rect, std::string theLabel);
		virtual ~cGuiButton();

		std::string * getLabel() {
			return label;
		}

	private:
		std::string * label;
};

#endif /* CGUIBUTTON_H_ */
