import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = 'data.csv'
data = pd.read_csv(file_path, delimiter=';')

data.columns = ['Name', 'Location', 'Company', 'Country', 'Gender', 'Birthdate', 'Wealth']

data['Wealth'] = pd.to_numeric(data['Wealth'], errors='coerce')

data['Birthdate'] = pd.to_datetime(data['Birthdate'], format='%d/%m/%Y %H:%M', errors='coerce')

data['YearOfBirth'] = data['Birthdate'].dt.year

top_20 = data.nlargest(20, 'Wealth')

palette = sns.color_palette("husl", len(top_20))

plt.figure(figsize=(14, 7))
sns.barplot(x='Wealth', y='Name', data=top_20, hue='Name', dodge=False, palette=palette, legend=False)
plt.title('Top 20 Wealthiest Individuals')
plt.xlabel('Wealth (in billions)')
plt.ylabel('Name')
plt.grid(True)
plt.show()

plt.figure(figsize=(8, 4))
sns.countplot(x='Gender', data=data, hue='Gender', palette='pastel', dodge=False)
plt.title('Gender Distribution')
plt.xlabel('Gender')
plt.ylabel('Count')
plt.grid(True)
plt.show()

plt.figure(figsize=(12, 6))
sns.scatterplot(x='YearOfBirth', y='Wealth', data=top_20, hue='Name', palette=palette, s=100)
plt.title('Wealth Distribution by Year of Birth (Top 20)')
plt.xlabel('Year of Birth')
plt.ylabel('Wealth (in billions)')
plt.legend(loc='upper left', bbox_to_anchor=(1, 1))
plt.grid(True)
plt.show()