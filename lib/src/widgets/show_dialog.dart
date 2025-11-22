import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import '../models/app_words.dart';
import '../models/contact.dart';

import '../pages/home_page.dart';

Future<Map<String, String>?> showAddContactDialog(BuildContext context) async {
  final formKey = GlobalKey<FormState>();
  final nameController = TextEditingController();
  final surnameController = TextEditingController();
  final nicknameController = TextEditingController();
  final phoneController = TextEditingController();
  final emailController = TextEditingController();

  final language = homePageKey.currentState!.appLanguage;

  return showDialog(
    context: context,
    builder: (BuildContext context) {
      return Center(
        child: SingleChildScrollView(
          child: AlertDialog(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
            title: Center(
              child: Text(
                appWords[language]!['add_contact']!,
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: homePageKey.currentState!.headerColor,
                ),
              ),
            ),
            content: Form(
              key: formKey,
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  TextFormField(
                    controller: nameController,
                    decoration: InputDecoration(
                      labelText: appWords[language]!['name']!,
                      prefixIcon: const Icon(Icons.person),
                      border: OutlineInputBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                    ),
                    validator: (value) {
                      if (value == null || value.trim().isEmpty) {
                        return appWords[language]!['name_error']!;
                      }
                      return null;
                    },
                  ),
                  const SizedBox(height: 12),
                  TextFormField(
                    controller: surnameController,
                    decoration: InputDecoration(
                      labelText: appWords[language]!['surname']!,
                      prefixIcon: const Icon(Icons.person_outline),
                      border: OutlineInputBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                    ),
                    validator: (value) {
                      if (value == null || value.isEmpty) {
                        return appWords[language]!['surname_error']!;
                      }
                      return null;
                    },
                    onChanged: (value) => surnameController.text = value.trim(),
                  ),
                  const SizedBox(height: 12),
                  TextFormField(
                    controller: nicknameController,
                    decoration: InputDecoration(
                      labelText:
                          '${appWords[language]!['nickname']!} (${appWords[language]!['opt']!})',
                      prefixIcon: const Icon(Icons.badge),
                      border: OutlineInputBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                    ),
                    validator: (value) {
                      if (value == null || value.trim().isEmpty) {
                        return null;
                      }
                      if (Contact.nicknameExists(value.trim())) {
                        return appWords[language]!['nickname_error']!;
                      }
                      return null;
                    },
                    onChanged: (value) =>
                        nicknameController.text = value.trim(),
                  ),
                  const SizedBox(height: 12),
                  TextFormField(
                    controller: phoneController,
                    decoration: InputDecoration(
                      labelText: appWords[language]!['phone_number']!,
                      hintText: '(555) 000 00 00',
                      hintStyle: const TextStyle(
                        color: Color.fromARGB(199, 220, 220, 220),
                      ),
                      counterText: "",
                      prefixIcon: const Icon(Icons.phone),
                      prefix: const Text(
                        '+90 ',
                        style: TextStyle(color: Colors.black, fontSize: 16),
                      ),
                      border: OutlineInputBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                    ),
                    maxLength: 14,
                    keyboardType: TextInputType.datetime,
                    autofillHints: const [AutofillHints.telephoneNumber],
                    inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                    onChanged: (String value) {
                      value = value.replaceAll(' ', '');

                      if (value.length > 10) {
                        if (value.startsWith('+90')) {
                          value = value.substring(3);
                        } else if (value.startsWith('90')) {
                          value = value.substring(2);
                        } else if (value.startsWith('0')) {
                          value = value.substring(1);
                        }
                      }

                      String newValue = value.replaceAll(RegExp(r'[^0-9]'), '');
                      if (newValue.length > 10) {
                        newValue = newValue.substring(0, 10);
                      }

                      if (newValue.length == 10) {
                        phoneController.text =
                            '(${newValue.substring(0, 3)}) ${newValue.substring(3, 6)} ${newValue.substring(6, 10)}';
                      } else if (newValue.length > 6) {
                        phoneController.text =
                            '(${newValue.substring(0, 3)}) ${newValue.substring(3, 6)} ${newValue.substring(6)}';
                      } else if (newValue.length > 3) {
                        phoneController.text =
                            '(${newValue.substring(0, 3)}) ${newValue.substring(3)}';
                      } else if (newValue.isNotEmpty) {
                        phoneController.text = '(${newValue.substring(0)}';
                      }
                    },
                    validator: (value) {
                      if (value == null || value.isEmpty) {
                        return appWords[language]!['phone_error']!;
                      }
                      if (value.length < 14) {
                        return appWords[language]!['phone_invalid_error']!;
                      }
                      if (Contact.phoneExists(
                        '+90${value.replaceAll(RegExp(r'[^0-9]'), '')}',
                      )) {
                        return appWords[language]!['phone_exists_error']!;
                      }
                      return null;
                    },
                  ),
                  const SizedBox(height: 12),
                  TextFormField(
                    controller: emailController,
                    decoration: InputDecoration(
                      labelText:
                          '${appWords[language]!['email']!} (${appWords[language]!['opt']!})',
                      prefixIcon: const Icon(Icons.email),
                      border: OutlineInputBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                    ),
                    autofillHints: const [AutofillHints.email],
                    keyboardType: TextInputType.emailAddress,
                    validator: (value) {
                      if (value == null || value.trim().isEmpty) {
                        return null;
                      }
                      final emailRegex = RegExp(r'^[^@]+@[^@]+\.[^@]+');
                      if (!emailRegex.hasMatch(value)) {
                        return appWords[language]!['email_error']!;
                      }
                      return null;
                    },
                    onChanged: (value) => emailController.text = value.trim(),
                  ),
                ],
              ),
            ),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(
                  appWords[language]!['cancel']!,
                  style: TextStyle(color: Colors.grey),
                ),
              ),
              ElevatedButton(
                onPressed: () {
                  if (formKey.currentState!.validate()) {
                    Navigator.of(context).pop({
                      "name": nameController.text.trim(),
                      "surname": surnameController.text,
                      "nickname": nicknameController.text,
                      "phoneNumber":
                          "+90${phoneController.text.replaceAll(RegExp(r'[^0-9]'), '')}",
                      "email": emailController.text,
                    });
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: homePageKey.currentState!.headerColor,
                  foregroundColor: Colors.white,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(appWords[language]!['add']!),
              ),
            ],
          ),
        ),
      );
    },
  );
}

Future<String?> showEditNameDialog(BuildContext context, String name) async {
  final formKey = GlobalKey<FormState>();
  final nameController = TextEditingController(text: name);

  final language = homePageKey.currentState!.appLanguage;

  return showDialog(
    context: context,
    builder: (BuildContext context) {
      return Center(
        child: SingleChildScrollView(
          child: AlertDialog(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
            title: Center(
              child: Text(
                appWords[language]!['edit_name']!,
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: homePageKey.currentState!.headerColor,
                ),
              ),
            ),
            content: Form(
              key: formKey,
              child: TextFormField(
                controller: nameController,
                decoration: InputDecoration(
                  labelText: appWords[language]!['name']!,
                  prefixIcon: const Icon(Icons.person),
                  border: OutlineInputBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                validator: (value) {
                  if (value == null || value.trim().isEmpty) {
                    return appWords[language]!['name_error']!;
                  }
                  return null;
                },
              ),
            ),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(
                  appWords[language]!['cancel']!,
                  style: TextStyle(color: Colors.grey),
                ),
              ),
              ElevatedButton(
                onPressed: () {
                  if (formKey.currentState!.validate()) {
                    Navigator.of(context).pop(nameController.text.trim());
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: homePageKey.currentState!.headerColor,
                  foregroundColor: Colors.white,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(appWords[language]!['save']!),
              ),
            ],
          ),
        ),
      );
    },
  );
}

Future<String?> showEditSurnameDialog(
  BuildContext context,
  String surname,
) async {
  final formKey = GlobalKey<FormState>();
  final surnameController = TextEditingController(text: surname);

  final language = homePageKey.currentState!.appLanguage;

  return showDialog(
    context: context,
    builder: (BuildContext context) {
      return Center(
        child: SingleChildScrollView(
          child: AlertDialog(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
            title: Center(
              child: Text(
                appWords[language]!['edit_surname']!,
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: homePageKey.currentState!.headerColor,
                ),
              ),
            ),
            content: SingleChildScrollView(
              child: Form(
                key: formKey,
                child: TextFormField(
                  controller: surnameController,
                  decoration: InputDecoration(
                    labelText: appWords[language]!['surname']!,
                    prefixIcon: const Icon(Icons.person_outline),
                    border: OutlineInputBorder(
                      borderRadius: BorderRadius.circular(8),
                    ),
                  ),
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return appWords[language]!['surname_error']!;
                    }
                    return null;
                  },
                  onChanged: (value) => surnameController.text = value.trim(),
                ),
              ),
            ),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(
                  appWords[language]!['cancel']!,
                  style: TextStyle(color: Colors.grey),
                ),
              ),
              ElevatedButton(
                onPressed: () {
                  if (formKey.currentState!.validate()) {
                    Navigator.of(context).pop(surnameController.text);
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: homePageKey.currentState!.headerColor,
                  foregroundColor: Colors.white,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(appWords[language]!['save']!),
              ),
            ],
          ),
        ),
      );
    },
  );
}

Future<String?> showEditNicknameDialog(
  BuildContext context,
  String nickname,
) async {
  final formKey = GlobalKey<FormState>();
  final nicknameController = TextEditingController(text: nickname);

  final language = homePageKey.currentState!.appLanguage;

  return showDialog(
    context: context,
    builder: (BuildContext context) {
      return Center(
        child: SingleChildScrollView(
          child: AlertDialog(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
            title: Center(
              child: Text(
                appWords[language]!['edit_nickname']!,
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: homePageKey.currentState!.headerColor,
                ),
              ),
            ),
            content: SingleChildScrollView(
              child: Form(
                key: formKey,
                child: TextFormField(
                  controller: nicknameController,
                  decoration: InputDecoration(
                    labelText:
                        '${appWords[language]!['nickname']!} (${appWords[language]!['opt']!})',
                    prefixIcon: const Icon(Icons.badge),
                    border: OutlineInputBorder(
                      borderRadius: BorderRadius.circular(8),
                    ),
                  ),
                  validator: (value) {
                    if (value == null || value.trim().isEmpty) {
                      return null;
                    }
                    if (Contact.nicknameExists(value.trim())) {
                      return appWords[language]!['nickname_error']!;
                    }
                    return null;
                  },
                  onChanged: (value) => nicknameController.text = value.trim(),
                ),
              ),
            ),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(
                  appWords[language]!['cancel']!,
                  style: TextStyle(color: Colors.grey),
                ),
              ),
              ElevatedButton(
                onPressed: () {
                  if (formKey.currentState!.validate()) {
                    Navigator.of(context).pop(nicknameController.text);
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: homePageKey.currentState!.headerColor,
                  foregroundColor: Colors.white,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(appWords[language]!['save']!),
              ),
            ],
          ),
        ),
      );
    },
  );
}

Future<String?> showEditPhoneNumberDialog(
  BuildContext context,
  String phoneNumber,
) async {
  final formKey = GlobalKey<FormState>();
  final phoneController = TextEditingController(
    text:
        '(${phoneNumber.substring(0, 3)}) ${phoneNumber.substring(3, 6)} ${phoneNumber.substring(6, 10)}',
  );

  final language = homePageKey.currentState!.appLanguage;

  return showDialog(
    context: context,
    builder: (BuildContext context) {
      return Center(
        child: SingleChildScrollView(
          child: AlertDialog(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
            title: Center(
              child: Text(
                appWords[language]!['edit_phone_number']!,
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: homePageKey.currentState!.headerColor,
                ),
              ),
            ),
            content: Form(
              key: formKey,
              child: TextFormField(
                controller: phoneController,
                decoration: InputDecoration(
                  labelText: appWords[language]!['phone_number']!,
                  hintText: '(555) 000 00 00',
                  hintStyle: const TextStyle(
                    color: Color.fromARGB(199, 220, 220, 220),
                  ),
                  counterText: "",
                  prefixIcon: const Icon(Icons.phone),
                  prefix: const Text(
                    '+90 ',
                    style: TextStyle(color: Colors.black, fontSize: 16),
                  ),
                  border: OutlineInputBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                maxLength: 14,
                keyboardType: TextInputType.datetime,
                autofillHints: const [AutofillHints.telephoneNumber],
                inputFormatters: [FilteringTextInputFormatter.digitsOnly],
                onChanged: (String value) {
                  value = value.replaceAll(' ', '');

                  if (value.length > 10) {
                    if (value.startsWith('+90')) {
                      value = value.substring(3);
                    } else if (value.startsWith('90')) {
                      value = value.substring(2);
                    } else if (value.startsWith('0')) {
                      value = value.substring(1);
                    }
                  }

                  String newValue = value.replaceAll(RegExp(r'[^0-9]'), '');
                  if (newValue.length > 10) {
                    newValue = newValue.substring(0, 10);
                  }

                  if (newValue.length == 10) {
                    phoneController.text =
                        '(${newValue.substring(0, 3)}) ${newValue.substring(3, 6)} ${newValue.substring(6, 10)}';
                  } else if (newValue.length > 6) {
                    phoneController.text =
                        '(${newValue.substring(0, 3)}) ${newValue.substring(3, 6)} ${newValue.substring(6)}';
                  } else if (newValue.length > 3) {
                    phoneController.text =
                        '(${newValue.substring(0, 3)}) ${newValue.substring(3)}';
                  } else if (newValue.isNotEmpty) {
                    phoneController.text = '(${newValue.substring(0)}';
                  }
                },
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return appWords[language]!['phone_error']!;
                  }
                  if (value.length < 14) {
                    return appWords[language]!['phone_invalid_error']!;
                  }
                  if (Contact.phoneExists(
                    '+90${value.replaceAll(RegExp(r'[^0-9]'), '')}',
                  )) {
                    return appWords[language]!['phone_exists_error']!;
                  }
                  return null;
                },
              ),
            ),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(
                  appWords[language]!['cancel']!,
                  style: TextStyle(color: Colors.grey),
                ),
              ),
              ElevatedButton(
                onPressed: () {
                  if (formKey.currentState!.validate()) {
                    Navigator.of(context).pop(
                      "+90${phoneController.text.replaceAll(RegExp(r'[^0-9]'), '')}",
                    );
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: homePageKey.currentState!.headerColor,
                  foregroundColor: Colors.white,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(appWords[language]!['save']!),
              ),
            ],
          ),
        ),
      );
    },
  );
}

Future<String?> showEditEmailDialog(BuildContext context, String email) async {
  final formKey = GlobalKey<FormState>();
  final emailController = TextEditingController(text: email);

  final language = homePageKey.currentState!.appLanguage;

  return showDialog(
    context: context,
    builder: (BuildContext context) {
      return Center(
        child: SingleChildScrollView(
          child: AlertDialog(
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(16),
            ),
            title: Center(
              child: Text(
                appWords[language]!['edit_email']!,
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  color: homePageKey.currentState!.headerColor,
                ),
              ),
            ),
            content: SingleChildScrollView(
              child: Form(
                key: formKey,
                child: TextFormField(
                  controller: emailController,
                  decoration: InputDecoration(
                    labelText:
                        '${appWords[language]!['email']!} (${appWords[language]!['opt']!})',
                    prefixIcon: const Icon(Icons.email),
                    border: OutlineInputBorder(
                      borderRadius: BorderRadius.circular(8),
                    ),
                  ),
                  autofillHints: const [AutofillHints.email],
                  keyboardType: TextInputType.emailAddress,
                  validator: (value) {
                    if (value == null || value.trim().isEmpty) {
                      return null;
                    }
                    final emailRegex = RegExp(r'^[^@]+@[^@]+\.[^@]+');
                    if (!emailRegex.hasMatch(value)) {
                      return appWords[language]!['email_error']!;
                    }
                    return null;
                  },
                  onChanged: (value) => emailController.text = value.trim(),
                ),
              ),
            ),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text(
                  appWords[language]!['cancel']!,
                  style: TextStyle(color: Colors.grey),
                ),
              ),
              ElevatedButton(
                onPressed: () {
                  if (formKey.currentState!.validate()) {
                    Navigator.of(context).pop(emailController.text);
                  }
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: homePageKey.currentState!.headerColor,
                  foregroundColor: Colors.white,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: Text(appWords[language]!['save']!),
              ),
            ],
          ),
        ),
      );
    },
  );
}

Future<Map<String, dynamic>?> showSettingsDialog(
  BuildContext context,
  String selectedLanguage,
  Color selectedColor,
) async {
  final List<Map<String, dynamic>> languages = [
    {'code': 'en', 'name': 'English', 'flag': '🇬🇧'},
    {'code': 'tr', 'name': 'Türkçe', 'flag': '🇹🇷'},
  ];

  final List<Map<String, dynamic>> themeColors = [
    {'name': 'Indigo', 'color': Colors.indigo},
    {'name': 'Blue', 'color': Colors.blue},
    {'name': 'Purple', 'color': Colors.purple},
    {'name': 'Teal', 'color': Colors.teal},
    {'name': 'Green', 'color': Colors.green},
    {'name': 'Orange', 'color': Colors.orange},
    {'name': 'Red', 'color': Colors.red},
    {'name': 'Pink', 'color': Colors.pink},
    {'name': 'Brown', 'color': Colors.brown},
  ];

  for (var colorData in themeColors) {
    if (colorData['color'].r == selectedColor.r &&
        colorData['color'].g == selectedColor.g &&
        colorData['color'].b == selectedColor.b) {
      selectedColor = colorData['color'] as Color;
      break;
    }
  }

  Map<String, Map<String, String>> translations = {
    'en': {
      'title': 'Settings',
      'language': 'Language',
      'themeColor': 'Theme Color',
      'preview': 'Preview',
      'previewText': 'Your app will look like this',
      'cancel': 'Cancel',
      'apply': 'Apply',
    },
    'tr': {
      'title': 'Ayarlar',
      'language': 'Dil',
      'themeColor': 'Tema Rengi',
      'preview': 'Önizleme',
      'previewText': 'Uygulamanız böyle görünecek',
      'cancel': 'İptal',
      'apply': 'Uygula',
    },
  };

  return showDialog<Map<String, dynamic>>(
    context: context,
    builder: (BuildContext context) {
      return StatefulBuilder(
        builder: (context, setState) {
          final t = translations[selectedLanguage]!;

          return Center(
            child: SingleChildScrollView(
              child: AlertDialog(
                shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(20),
                ),
                title: Row(
                  children: [
                    Icon(Icons.settings, color: selectedColor),
                    const SizedBox(width: 12),
                    Text(
                      t['title']!,
                      style: const TextStyle(fontWeight: FontWeight.bold),
                    ),
                  ],
                ),
                content: SingleChildScrollView(
                  child: Column(
                    mainAxisSize: MainAxisSize.min,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        t['language']!,
                        style: TextStyle(
                          fontSize: 16,
                          fontWeight: FontWeight.w600,
                          color: selectedColor,
                        ),
                      ),
                      const SizedBox(height: 12),
                      Container(
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.grey.shade300),
                          borderRadius: BorderRadius.circular(12),
                        ),
                        child: Column(
                          children: languages.map((lang) {
                            final isSelected = selectedLanguage == lang['code'];
                            return InkWell(
                              onTap: () {
                                setState(() {
                                  selectedLanguage = lang['code'] as String;
                                });
                              },
                              borderRadius: BorderRadius.circular(12),
                              child: Container(
                                padding: const EdgeInsets.symmetric(
                                  horizontal: 16,
                                  vertical: 12,
                                ),
                                decoration: BoxDecoration(
                                  color: isSelected
                                      ? selectedColor.withAlpha(25)
                                      : Colors.transparent,
                                  borderRadius: BorderRadius.circular(12),
                                ),
                                child: Row(
                                  children: [
                                    Text(
                                      lang['flag'] as String,
                                      style: const TextStyle(fontSize: 24),
                                    ),
                                    const SizedBox(width: 12),
                                    Expanded(
                                      child: Text(
                                        lang['name'] as String,
                                        style: TextStyle(
                                          fontSize: 16,
                                          fontWeight: isSelected
                                              ? FontWeight.w600
                                              : FontWeight.normal,
                                          color: isSelected
                                              ? selectedColor
                                              : Colors.black87,
                                        ),
                                      ),
                                    ),
                                    if (isSelected)
                                      Icon(
                                        Icons.check_circle,
                                        color: selectedColor,
                                        size: 24,
                                      ),
                                  ],
                                ),
                              ),
                            );
                          }).toList(),
                        ),
                      ),
                      const SizedBox(height: 24),
                      Text(
                        t['themeColor']!,
                        style: TextStyle(
                          fontSize: 16,
                          fontWeight: FontWeight.w600,
                          color: selectedColor,
                        ),
                      ),
                      const SizedBox(height: 12),
                      Wrap(
                        spacing: 12,
                        runSpacing: 12,
                        children: themeColors.map((colorData) {
                          final color = colorData['color'] as Color;
                          final isSelected = selectedColor == color;
                          return GestureDetector(
                            onTap: () {
                              setState(() {
                                selectedColor = color;
                              });
                            },
                            child: Container(
                              width: 56,
                              height: 56,
                              decoration: BoxDecoration(
                                color: color,
                                shape: BoxShape.circle,
                                border: Border.all(
                                  color: isSelected
                                      ? Colors.black87
                                      : Colors.grey.shade300,
                                  width: isSelected ? 3 : 2,
                                ),
                                boxShadow: [
                                  if (isSelected)
                                    BoxShadow(
                                      color: color.withAlpha(102),
                                      blurRadius: 8,
                                      spreadRadius: 2,
                                    ),
                                ],
                              ),
                              child: isSelected
                                  ? const Icon(
                                      Icons.check,
                                      color: Colors.white,
                                      size: 28,
                                    )
                                  : null,
                            ),
                          );
                        }).toList(),
                      ),
                      const SizedBox(height: 16),

                      Container(
                        padding: const EdgeInsets.all(16),
                        decoration: BoxDecoration(
                          color: selectedColor.withAlpha(25),
                          borderRadius: BorderRadius.circular(12),
                          border: Border.all(
                            color: selectedColor.withAlpha(78),
                          ),
                        ),
                        child: Row(
                          children: [
                            Icon(Icons.preview, color: selectedColor),
                            const SizedBox(width: 12),
                            Expanded(
                              child: Column(
                                crossAxisAlignment: CrossAxisAlignment.start,
                                children: [
                                  Text(
                                    t['preview']!,
                                    style: const TextStyle(
                                      fontSize: 12,
                                      color: Colors.grey,
                                    ),
                                  ),
                                  FittedBox(
                                    fit: BoxFit.scaleDown,
                                    child: Text(
                                      t['previewText']!,
                                      style: TextStyle(
                                        fontSize: 14,
                                        fontWeight: FontWeight.w500,
                                        color: selectedColor,
                                      ),
                                    ),
                                  ),
                                ],
                              ),
                            ),
                          ],
                        ),
                      ),
                    ],
                  ),
                ),
                actions: [
                  TextButton(
                    onPressed: () {
                      Navigator.of(context).pop();
                    },
                    child: Text(
                      t['cancel']!,
                      style: TextStyle(color: Colors.grey.shade600),
                    ),
                  ),
                  ElevatedButton(
                    onPressed: () {
                      Navigator.of(context).pop({
                        'language': selectedLanguage,
                        'color': selectedColor,
                      });
                    },
                    style: ElevatedButton.styleFrom(
                      backgroundColor: selectedColor,
                      foregroundColor: Colors.white,
                      shape: RoundedRectangleBorder(
                        borderRadius: BorderRadius.circular(8),
                      ),
                      padding: const EdgeInsets.symmetric(
                        horizontal: 24,
                        vertical: 12,
                      ),
                    ),
                    child: Text(t['apply']!),
                  ),
                ],
              ),
            ),
          );
        },
      );
    },
  );
}
