import matplotlib.pyplot as plt
from fpdf import FPDF

# plt.plot([1, 2, 3], [1, 4, 9])
# plt.plot([1, 2, 3], [1, 5, 9])
# plt.savefig('foo.png')

name = "example.pdf"

pdf = FPDF()
pdf.add_page()

pdf.set_font('Arial', '', 16)
pdf.cell(w=0, h=10, txt="This is regular text.", ln=1)
pdf.set_font('Arial', 'B', 16)
pdf.cell(w=0, h=10, txt="This is bold text.", ln=1)
pdf.set_font('Arial', 'I', 16)
pdf.cell(w=0, h=10, txt="This is italic text.", ln=1)
pdf.set_font('Arial', '', 16)  # Reset text back to regular
pdf.cell(w=0, h=10, txt="This is left aligned text.", ln=1,
         align='L')
pdf.cell(w=0, h=10, txt="This is center aligned text.", ln=1,
         align='C')
pdf.cell(w=0, h=10, txt="This is right aligned text.", ln=1,
         align='R')
pdf.set_fill_color(r=0, g=128, b=0)
pdf.cell(w=0, h=10, txt="This is text with filled background.", ln=1,
         fill=True)
pdf.set_text_color(r=0, g=128, b=0)
pdf.cell(w=0, h=10, txt="This is colored text.", ln=1)

pdf.output(name, "F")
