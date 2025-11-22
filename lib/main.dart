import 'package:flutter/material.dart';

import 'src/pages/home_page.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Hangouts',
      debugShowCheckedModeBanner: false,
      home: HomePage(key: homePageKey),
    );
  }
}
